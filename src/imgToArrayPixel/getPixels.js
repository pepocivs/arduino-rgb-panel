const Jimp = require('jimp');
const fs = require('fs');

const configs = {
  ledMatrix: {
    imagePath: 'images/64x64/',
    zigzag: false,
    is32Bits: true,
    convertTo128Width: true,
    splitImages: true,
    isolatedFrames: false,
  },
  ledStrip: {
    imagePath: 'images/25x21/',
    zigzag: true,
    is32Bits: false,
    convertTo128Width: false,
    splitImages: false,
    isolatedFrames: false,
  },
  ledStripBig: {
    imagePath: 'images/34x34/',
    zigzag: false,
    is32Bits: false,
    convertTo128Width: false,
    splitImages: false,
    isolatedFrames: false,
  }
}

const config = configs.ledStripBig;

// Helper functions
const getHex = (str) => {
  const convertTo32 = (config.is32Bits) ? '00': '';
  const hexNumber = (("0000000" + ((str|0)+4294967296).toString(16)).substr(-8)).slice(0, -2);
  return `0x${convertTo32}${hexNumber}`;
};

const checkExtension = (fileName) => {
  const validExt = ['png', 'jpg', 'bmp'];
  const ext = fileName.split('.').pop();
  return !!(validExt.indexOf(ext) > -1);
};
const sortByKey = (array, key) => {
  return array.sort(function (a, b) {
    var x = a[key]; var y = b[key];
    return ((x < y) ? -1 : ((x > y) ? 1 : 0));
  });
};

const getImageColors = (str, img, x, y) => `${str}${getHex(img.getPixelColor(x, y))}, `;

const getPixels = (image) => {
  const imageWidth = image.bitmap.width;
  const imageHeight = (config.convertTo128Width) ? image.bitmap.height / 2 : image.bitmap.height;
  let imageColors = '';
  let imageColorsA = '';
  let imageColorsB = '';
  let firstScreenColors = '';
  let secondScreenColors = '';
  for (let y = 0; y < imageHeight; y++) {
    firstScreenColors = '';
    secondScreenColors = '';
    if (y !== 0 && y % 2 !== 0 && config.zigzag) {
      for (let x = imageWidth - 1; x >= 0; x--) {
        imageColors = getImageColors(imageColors, image, x, y);
        if (y < 32) imageColorsA = getImageColors(imageColorsA, image, x, y);
               else imageColorsB = getImageColors(imageColorsB, image, x, y);
      }
    } else {
      for (let x = 0; x < imageWidth; x++) {
        if (config.convertTo128Width) {
          firstScreenColors = getImageColors(firstScreenColors, image, x, y);
          secondScreenColors = getImageColors(secondScreenColors, image, x, y + imageHeight);
        } else {
          imageColors = getImageColors(imageColors, image, x, y);
        }
        if (y < 32) imageColorsA = getImageColors(imageColorsA, image, x, y);
               else imageColorsB = getImageColors(imageColorsB, image, x, y);
      }

      
    }
    imageColors = `${imageColors}${firstScreenColors}${secondScreenColors}\n`;
    if (y < 32) imageColorsA = `${imageColorsA}\n`;
           else imageColorsB = `${imageColorsB}\n`;
  }
  imageColors = imageColors.slice(0, -3);
  imageColorsA = imageColorsA.slice(0, -3);
  imageColorsB = imageColorsB.slice(0, -3);
  return { imageColors, imageColorsA, imageColorsB };
};
const getArrayOfPixels = (filePath) => {
  return Jimp.read(filePath)
    .then(image => {
      image.color([
        { apply: 'saturate', params: [25] },
        { apply: 'darken', params: [15] }, 
      ]);
      return {
        totalSize: (image.bitmap.width * image.bitmap.height),
        width: image.bitmap.width,
        height: image.bitmap.height,
        colorString: getPixels(image).imageColors,
        colorStringA: getPixels(image).imageColorsA,
        colorStringB: getPixels(image).imageColorsB,
      }
    })
    .catch(err => { throw err });
}
const generateDeclaration = (animationName, frameNumber, pixelImages, splitDivisor) => {
  const secondLevel = (typeof frameNumber === 'number') ? '' : '[]';
  const secondLevelClose = (typeof frameNumber === 'number') ? '' : '{';
  return `const long ${animationName}${frameNumber}${secondLevel}[${pixelImages[animationName][0].totalSize/splitDivisor}] PROGMEM = ${secondLevelClose}\n`;
}

// Core functions
const generateFileString = (filePart, animationName, pixelImages, splitDivisor) => {
    let propertiesVar = '';
    const secondLevelClose = (config.isolatedFrames) ? '' : '\n}';
    const secondLevelEnd = (config.isolatedFrames) ? ';' : ',';
    let hFile = (config.isolatedFrames) ? '' : generateDeclaration(animationName, '', pixelImages, splitDivisor);
    const sortedFrames = sortByKey(pixelImages[animationName], 'fileName');
    sortedFrames.map((frame, index) => {
      hFile = `${hFile}${(config.isolatedFrames) ? generateDeclaration(animationName, index, pixelImages, splitDivisor) : ''}`;
      hFile = `${hFile}{\n${frame[`colorString${filePart}`]}}${secondLevelEnd}\n`;
      propertiesVar = `
        \nconst int ${animationName}Width = ${(config.convertTo128Width) ? 128 : frame.width};
        \nconst int ${animationName}Height = ${(config.convertTo128Width) ? 32 : frame.height / splitDivisor};
      `;
    });
    hFile = `${hFile.slice(0, -3)}${secondLevelClose}\n};`;
    hFile = `${hFile}\nconst int ${animationName}Frames = ${sortedFrames.length};${propertiesVar}`;
    fs.writeFileSync(`${config.imagePath}${animationName}/${animationName}${filePart}.h`, hFile); 
    console.log(`Saved: ${config.imagePath}${animationName}/${animationName}${filePart}.h`);
}

const generateFiles = async (pixelImages) => {
  Object.keys(pixelImages).map(animationName => {
    generateFileString('', animationName, pixelImages, 1);
    if (config.splitImages) {
      generateFileString('A', animationName, pixelImages, 2);
      generateFileString('B', animationName, pixelImages, 2);
    }
  });
};
const walkDirectories = function (dir, animationName = '') {
  const promises = [];
  const pixelImages = {};
  files = fs.readdirSync(dir);
  files.map(function (fileName) {
    if (fs.statSync(dir + fileName).isDirectory()) {
      walkDirectories(dir + fileName + '/', fileName);
    } else if (checkExtension(fileName)) {
      const filePath = `${dir}${fileName}`;
      promises.push(
        getArrayOfPixels(filePath).then(imageInfo => {
          if (!pixelImages[animationName]) pixelImages[animationName] = [];
          pixelImages[animationName].push({ ...imageInfo, fileName });
        })
      );
    }
  });
  Promise.all(promises).then(() => {
    generateFiles(pixelImages);
  });
};

walkDirectories(config.imagePath);
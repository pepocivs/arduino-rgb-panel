const Jimp = require('jimp');
const fs = require('fs');

const configs = {
  ledMatrix: {
    imagePath: 'images/64x64/',
    zigzag: false,
    is32Bits: true,
    splitImages: true,
  },
  ledStrip: {
    imagePath: 'images/25x21/',
    zigzag: true,
    is32Bits: false,
    splitImages: false,
  }
}

const config = configs.ledMatrix;

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
  const imageHeight = image.bitmap.height;
  let imageColors = '';
  let imageColorsA = '';
  let imageColorsB = '';
  for (let y = 0; y < imageHeight; y++) {
    if (y !== 0 && y % 2 !== 0 && config.zigzag) {
      for (let x = imageWidth - 1; x >= 0; x--) {
        imageColors = getImageColors(imageColors, image, x, y);
        if (y < 32) imageColorsA = getImageColors(imageColorsA, image, x, y);
               else imageColorsB = getImageColors(imageColorsA, image, x, y);
      }
    } else {
      for (let x = 0; x < imageWidth; x++) {
        imageColors = getImageColors(imageColors, image, x, y);
        if (y < 32) imageColorsA = getImageColors(imageColorsA, image, x, y);
               else imageColorsB = getImageColors(imageColorsA, image, x, y);
      }
    }
    imageColors = `${imageColors}\n`;
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

// Core functions
const generateFileString = (filePart, animationName, pixelImages, splitDivisor) => {
    let propertiesVar = '';
    let hFile = `const long ${animationName}[][${pixelImages[animationName][0].totalSize/splitDivisor}] PROGMEM = {\n`;
    const sortedFrames = sortByKey(pixelImages[animationName], 'fileName');
    sortedFrames.map(frame => {
      hFile = `${hFile}{\n${frame[`colorString${filePart}`]}},\n`;
      propertiesVar = `
        \nconst int ${animationName}Width = ${frame.width};
        \nconst int ${animationName}Height = ${frame.height / splitDivisor};
      `;
    });
    hFile = `${hFile.slice(0, -3)}\n}\n};`;
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
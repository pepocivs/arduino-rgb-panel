const Jimp = require('jimp');
const fs = require('fs');

const imagePath = 'images/';

// Helper Functions
const hasSpecialBrightness = (percentileChange, x, y) => (percentileChange && y > 8 && !((y === 20) && (x === 24)));

const calculateBrightness = (hexNumber, percentileChange) => {
  const add = true;
  const maxValue = 255;
  const minValue = 0;
  const additionValue = Math.floor(maxValue*percentileChange/100);
  const red = parseInt(hexNumber.substring(0, 2), 16);
  const green = parseInt(hexNumber.substring(2, 4), 16);
  const blue = parseInt(hexNumber.substring(4, 6), 16);

  let newRed
  let newGreen
  let newBlue

  if (add) {
    newRed = ((red + additionValue) > maxValue) ? maxValue :  (red + additionValue);
    newGreen = ((green + additionValue) > maxValue) ? maxValue :  (green + additionValue);
    newBlue = ((blue + additionValue) > maxValue) ? maxValue :  (blue + additionValue);
  } else {
    newRed = ((red - additionValue) < minValue) ? minValue :  (red - additionValue);
    newGreen = ((green - additionValue) < minValue) ? minValue :  (green - additionValue);
    newBlue = ((blue - additionValue) < minValue) ? minValue :  (blue - additionValue);
  }

  return `${toHex(newRed)}${toHex(newGreen)}${toHex(newBlue)}`;
}
function toHex(d) {
  return  ("0"+(Number(d).toString(16))).slice(-2);
}

const getHex = (str, percentileChange, x, y) => {
  const hexNumber = (("0000000" + ((str|0)+4294967296).toString(16)).substr(-8)).slice(0, -2);
  return (hasSpecialBrightness(percentileChange, x, y)) 
    ? `0x${calculateBrightness(hexNumber, percentileChange)}`
    : `0x${hexNumber}`;
};
const getRealHex = (str, percentileChange, x, y) => {
  const hexNumber = (("0000000" + ((str|0)+4294967296).toString(16)).substr(-8));
  return (hasSpecialBrightness(percentileChange, x, y)) 
    ? `${calculateBrightness(hexNumber, percentileChange)}ff`
    : hexNumber;
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

// Getter functions
const createResultImage = (image, percentileChange) => {
  const imageWidth = image.bitmap.width;
  const imageHeight = image.bitmap.height;
  new Jimp(imageWidth, imageHeight, (err, newImage) => {
    for (let y = 0; y < imageHeight; y++) {
      for (let x = 0; x < imageWidth; x++) {
        newImage.setPixelColor(parseInt(getRealHex(image.getPixelColor(x, y), percentileChange, x, y), 16), x, y);
      }
    }
    newImage.write(`images/generated/${percentileChange.toString().padStart(3, '0')}test.png`);
  });
}

const getPixels = (image, percentileChange) => {
  const imageWidth = image.bitmap.width;
  const imageHeight = image.bitmap.height;
  let imageColors = '';
  for (let y = 0; y < imageHeight; y++) {
    if (y !== 0 && y % 2 !== 0) {
      for (let x = imageWidth - 1; x >= 0; x--) imageColors = `${imageColors}${getHex(image.getPixelColor(x, y), percentileChange, x, y)}, `;
    } else {
      for (let x = 0; x < imageWidth; x++) imageColors = `${imageColors}${getHex(image.getPixelColor(x, y), percentileChange, x, y)}, `;
    }
    imageColors = `${imageColors}\n`;
  }
  imageColors = imageColors.slice(0, -3);
  return imageColors;
};
const getArrayOfPixels = (filePath, percentileChange, generate) => {
  return Jimp.read(filePath)
    .then(image => {
      if (generate) createResultImage(image, percentileChange);
      return {
        totalSize: (image.bitmap.width * image.bitmap.height),
        colorString: getPixels(image, percentileChange),
      }
    })
    .catch(err => { throw err });
}

// Core functions
const generateFiles = async (pixelImages) => {
  Object.keys(pixelImages).map(animationName => {
    let hFile = `const long ${animationName}[][${pixelImages[animationName][0].totalSize}] PROGMEM = {\n`;
    const sortedFrames = sortByKey(pixelImages[animationName], 'fileName');
    sortedFrames.map(frame => {
      hFile = `${hFile}{\n${frame.colorString}},\n`;
    });
    hFile = `${hFile.slice(0, -3)}\n}\n};`;
    hFile = `${hFile}\nconst int ${animationName}Frames = ${sortedFrames.length};`;
    fs.writeFileSync(`${imagePath}${animationName}/${animationName}.h`, hFile); 
    console.log(`Saved: ${imagePath}${animationName}/${animationName}.h`);
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
          pixelImages[animationName].push({
            fileName,
            totalSize: imageInfo.totalSize,
            colorString: imageInfo.colorString
          });
        })
      );
    }
  });
  Promise.all(promises).then(() => {
    generateFiles(pixelImages);
  });
};

const cromaticTest = () => {
  const promises = [];
  const imageSet = [];
  const iterations = 100;
  const increment = 1;
  for (let cont = 0; cont < iterations; cont ++) {
    promises.push(
      getArrayOfPixels('images/cromatic/cromatic.png', (cont * increment), true).then(imageInfo => {
        imageSet.push({
          fileName: `cromaticTest${cont}`,
          totalSize: imageInfo.totalSize,
          colorString: imageInfo.colorString
        });
      })
    );
  }
  Promise.all(promises).then(() => {
    generateFiles({cromatic: imageSet});
  });
}

// walkDirectories(imagePath);
cromaticTest();

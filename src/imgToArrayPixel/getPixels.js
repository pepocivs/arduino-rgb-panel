const Jimp = require('jimp');
const fs = require('fs');

const imagePath = 'images/';

// Helper Functions
const getHex = (str) => {
  return (str <= 255) ? '0x000000' : `0x${str.toString(16).slice(0, -2)}`;
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
const getPixels = (image) => {
  const imageWidth = image.bitmap.width;
  const imageHeight = image.bitmap.height;
  let imageColors = '';
  for (let y = 0; y < imageHeight; y++) {
    if (y !== 0 && y % 2 !== 0) {
      for (let x = imageWidth - 1; x >= 0; x--) imageColors = `${imageColors}${getHex(image.getPixelColor(x, y))}, `;
    } else {
      for (let x = 0; x < imageWidth; x++) imageColors = `${imageColors}${getHex(image.getPixelColor(x, y))}, `;
    }
    imageColors = `${imageColors}\n`;
  }
  imageColors = imageColors.slice(0, -3);
  return imageColors;
};
const getArrayOfPixels = (filePath) => {
  return Jimp.read(filePath)
    .then(image => {
      return {
        totalSize: (image.bitmap.width * image.bitmap.height),
        colorString: getPixels(image),
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

walkDirectories(imagePath);

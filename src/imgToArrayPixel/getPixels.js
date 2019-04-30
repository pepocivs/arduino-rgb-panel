const Jimp = require('jimp');
const fs = require('fs');

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
    .then(image => getPixels(image))
    .catch(err => { throw err });
}

// Core functions
const generateFiles = (pixelImages) => {
  Object.keys(pixelImages).map(animationName => {
    console.log(animationName);
    const sortedFrames = sortByKey(pixelImages[animationName], 'fileName');
    sortedFrames.map(frame => {
      console.log(frame.fileName);
      console.log(frame.colorString);
    });
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
        getArrayOfPixels(filePath).then(colorString => {
          if (!pixelImages[animationName]) pixelImages[animationName] = [];
          pixelImages[animationName].push({
            fileName,
            colorString
          });
        })
      );
    }
  });
  Promise.all(promises).then(() => {
    generateFiles(pixelImages);
  });
};

walkDirectories('images/');

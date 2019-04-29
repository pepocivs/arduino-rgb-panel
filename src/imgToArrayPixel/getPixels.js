const Jimp = require('jimp');
const fs = require('fs');

const getHex = (str) => {
  return `0x${str.toString(16).slice(0, -2)}`;
}

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
}

const getArrayOfPixels = (filePath) => {
  return Jimp.read(filePath)
    .then(image => getPixels(image))
    .catch(err => { throw err });
}

const checkExtension = (fileName) => {
  const validExt = ['png', 'jpg', 'bmp'];
  const ext = fileName.split('.').pop();
  return !!(validExt.indexOf(ext) > -1);
}

const walkDirectories = function (dir, animationName = '') {
  files = fs.readdirSync(dir);
  files.forEach(function (fileName) {
    if (fs.statSync(dir + fileName).isDirectory()) {
      walkDirectories(dir + fileName + '/', fileName);
    } else if (checkExtension(fileName)) {
      const filePath = `${dir}${fileName}`;
      console.log(animationName, filePath);
      getArrayOfPixels(filePath).then(txt => {
        console.log(animationName, '->', fileName);
        console.log(txt);
        console.log('-----');
      });
    }
  });
};

walkDirectories('images/');
// getArrayOfPixels('images/pikawalk/pika01.png').then(txt => console.log(txt));

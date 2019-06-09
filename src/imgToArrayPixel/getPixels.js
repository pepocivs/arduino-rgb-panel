const Jimp = require('jimp');
const fs = require('fs');

const configs = {
  ledMatrix: {
    imagePath: 'images/64x64/',
    zigzag: false,
    rgb: true,
  },
  ledStrip: {
    imagePath: 'images/25x21/',
    zigzag: true,
    rgb: false,
  }
}

const config = configs.ledStrip;


// Helper functions
const getHex = (str) => {
  const hexNumber = (("0000000" + ((str|0)+4294967296).toString(16)).substr(-8)).slice(0, -2);
  return (config.rgb) ? getRGB(hexNumber): `0x${hexNumber}`;
};
const getRGB = (hex) => {
  return `{0x${hex.substr(0, 2)}, 0x${hex.substr(2, 2)}, 0x${hex.substr(4, 2)}}`
}
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

const getPixels = (image) => {
  const imageWidth = image.bitmap.width;
  const imageHeight = image.bitmap.height;
  let imageColors = '';
  for (let y = 0; y < imageHeight; y++) {
    if (y !== 0 && y % 2 !== 0 && config.zigzag) {
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
        width: image.bitmap.width,
        height: image.bitmap.height,
        colorString: getPixels(image),
      }
    })
    .catch(err => { throw err });
}

// Core functions
const generateFiles = async (pixelImages) => {
  Object.keys(pixelImages).map(animationName => {
    let propertiesVar = '';
    const rgbDef = (config.rgb) ? '[3]' : '';
    const typeVar = (config.rgb) ? 'uint8_t' : 'long';
    let hFile = `const ${typeVar} ${animationName}[][${pixelImages[animationName][0].totalSize}]${rgbDef} PROGMEM = {\n`;
    const sortedFrames = sortByKey(pixelImages[animationName], 'fileName');
    sortedFrames.map(frame => {
      hFile = `${hFile}{\n${frame.colorString}},\n`;
      propertiesVar = `
        \nconst int ${animationName}Width = ${frame.width};
        \nconst int ${animationName}Height = ${frame.height};
      `;
    });
    hFile = `${hFile.slice(0, -3)}\n}\n};`;
    hFile = `${hFile}\nconst int ${animationName}Frames = ${sortedFrames.length};${propertiesVar}`;
    fs.writeFileSync(`${config.imagePath}${animationName}/${animationName}.h`, hFile); 
    console.log(`Saved: ${config.imagePath}${animationName}/${animationName}.h`);
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
            width: imageInfo.width,
            height: imageInfo.height,
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

walkDirectories(config.imagePath);

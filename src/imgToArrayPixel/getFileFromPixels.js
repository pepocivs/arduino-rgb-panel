const Jimp = require('jimp');
const fs = require('fs');


const hPath = './images/64x64';
const fileName = 'bigTriforce'
const width = 128;
const height = 32;

function readModuleFile(path, callback) {
  try {
      var filename = require.resolve(path);
      fs.readFile(path, 'utf8', callback);
  } catch (e) {
      callback(e);
  }
}

readModuleFile(`${hPath}/${fileName}/${fileName}.h`, function (err, words) {
  let imagePixels = words.split('\n{')[1].split('},')[0].split(', ');
  let count = 0;
  imagePixels = imagePixels.map((pixel) => {
    return pixel.replace(new RegExp('\r\n', 'g'), '').substring(4);
  });

  
  new Jimp(width, height, 'green', (err, image) => {
    if (err) throw err;
      for (let y = 0; y < height; y ++) {
        for (let x = 0; x < width; x ++) {
          image.setPixelColor(parseInt(`0x${imagePixels[count]}`), x, y);
          count ++;
        }
      }
      image.write(`images/generated/${fileName}/${fileName}-generated.png`);
  });
        
});



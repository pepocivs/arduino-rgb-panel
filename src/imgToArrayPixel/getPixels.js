const Jimp = require('jimp');

let documentName = process.argv[2] || 'default';


const walkSync = function (dir, filelist) {
  var fs = fs || require('fs'),
  files = fs.readdirSync(dir);
  filelist = filelist || [];
  files.forEach(function (file) {
    if (fs.statSync(dir + file).isDirectory()) {
      filelist = walkSync(dir + file + '/', filelist);
    }
    else {
      filelist.push(file);
    }
  });
  return filelist;
};

console.log(walkSync('images/'));

Jimp.read("images/haunter/haunter.png", function (err, image) {
  if (err) { console.error(err); return; }
  console.log(image.getPixelColor(0, 0).toString(16));
});
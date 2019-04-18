/*
 * top to bottom
 * backward - forward
*/

for (var y = 0; y < image.height; y++) {
  if(y !== 0 && y%2 !== 0) {
    for (var x = 0; x < image.width; x++) {
      image.addPoint(x, y);
    }
  } else {
    for (var x = image.width - 1; x >= 0; x--) {
      image.addPoint(x, y);
    }
  }
}

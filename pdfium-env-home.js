const os = require('os');
const path = require('path');
let home = ''

if (os.platform() !== 'win32') {
    throw Error('Currently we do not support non-win platform');
}

if (process.env.PDFIUM_HOME) {
    home = process.env.PDFIUM_HOME;
} else {
    home = path.join(__dirname, os.arch());
}

process.stdout.write(home)
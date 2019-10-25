const pdfium = require('../');
const path = require('path');

pdfium.printPDF({
    filePath: path.join(__dirname, '111.pdf'),
    printerName: 'DASCOM DL-210Z',
    dpi: 203, 
    width: 800,
    height: 800,
    blackWhite: 1
})
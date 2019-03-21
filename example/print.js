const pdfium = require('../');
const path = require('path');

pdfium.printPDF({
    filePath: path.join(__dirname, 'ruston.pdf'),
    printerName: 'DASCOM DL-210Z',
    dpi: 203, 
    width: 800,
    height: 800,
})
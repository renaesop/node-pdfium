const pdfium = require('../');
const path = require('path');

pdfium.printPDF({
    filePath: path.join(__dirname, 'fr.pdf'),
    printerName: 'DASCOM DL-210Z',
    dpi: 200, 
    width: 700 * 1.01,
    height: 700 * 1.5 * 1.01
})
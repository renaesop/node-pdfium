const path = require('path');
const printer = require('printer');

const pdfium = require('../lib/pdfium');

describe('basic pdfium use', () => {
    it('should work and not throw', async () => {
        const printers = printer.getPrinters();
        console.log(printers);
        await pdfium.printPDF({
            filePath: path.join(__dirname, '..', 'example', 'usps.pdf'),
            printerName: printers[0].name,
        })
    });
});

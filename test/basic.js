const path = require('path');

const pdfium = require('../');

describe('basic pdfium use', () => {
    it('should work and not throw', async () => {
        await pdfium.printPDF({
            filePath: path.join(__dirname, '..', 'example', 'usps.pdf'),
            printerName: 'Microsoft Print to PDF',
            dpi: 203, 
            width: 800,
            height: 800,
        })
    });
});

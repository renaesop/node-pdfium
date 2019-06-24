const path = require('path');

const pdfium = require('../lib/pdfium');

describe('basic pdfium use', () => {
    it('should work and not throw', async () => {
        await pdfium.printPDF({
            filePath: path.join(__dirname, '..', 'example', 'usps.pdf'),
            printerName: 'Microsoft Print to PDF',
        })
    });
});

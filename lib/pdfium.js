const pdfium = require('bindings')({ bindings: 'pdfium.node', module_root: __dirname });

exports.printPDF = function printPDF({
    pageList = [],
    width = 0,
    height = 0,
    dpi = 324,
    copies = 1,
    filePath,
    printerName,
}) {
    return new Promise((resolve, reject) => {
        try {
            pdfium.printPDF(printerName, filePath, {
                pageList,
                width,
                height,
                dpi,
                copies,
            });
            resolve(null);
        } catch (e) {
            reject(e);
        }
    });
};

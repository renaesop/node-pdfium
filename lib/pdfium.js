const pdfium = require('bindings')('pdfium.node');

exports.printPDF = function printPDF({
    pageList,
    width,
    height,
    dpi,
    copies,
    
    filePath,

    printerName,
}) {
    return new Promise((resolve, reject) => {

    });
};

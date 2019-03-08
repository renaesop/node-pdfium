const pdfium = require('bindings')('pdfium.node');

pdfium.printPDF("DASCOM DL-210Z", require('path').join(__dirname, '..', '123.pdf'), {
    pageList: [],
    width: 0,
    height: 0,
    dpi: 203,
    copies: 1,
})

console.log(123)

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
        pdfium.printPDF("DASCOM DL-210Z", require('path').join(__dirname, '..', '123.pdf'), {
            pageList: [],
            width: 0,
            height: 0,
            dpi: 203,
            copies: 1,
        })
    });
};

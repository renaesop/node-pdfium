#ifndef PRINTER_WIN
#define PRINTER_WIN

#include "inc.h"

namespace node_pdfium {
    struct DCDeleter {
        inline void operator()(HDC dc) { DeleteDC(dc); }
    };
    using Unique_HDC =
        std::unique_ptr<std::remove_pointer<HDC>::type, DCDeleter>;
    Unique_HDC GetPrinterDC(const v8::Local<v8::Value>& printerName);
};

#endif
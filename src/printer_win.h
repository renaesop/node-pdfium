#ifndef PRINTER_WIN
#define PRINTER_WIN

#include "inc.h"

namespace node_pdfium {
    struct DCDeleter {
        inline void operator()(HDC dc) { ::DeleteDC(dc); }
    };
    struct HPDeleter {
        inline void operator()(HANDLE h) { ::ClosePrinter(h); }
    };
    using Unique_HDC =
        std::unique_ptr<std::remove_pointer<HDC>::type, DCDeleter>;
    using Unique_HPrinter =
        std::unique_ptr<std::remove_pointer<HANDLE>::type, HPDeleter>;     
    Unique_HDC GetPrinterDC(const v8::Local<v8::Value>& printerName);
    Unique_HPrinter GetPrinterHanlde(const v8::Local<v8::Value>& printerName);
};

#endif
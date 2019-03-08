#include <wingdi.h>
#include <nan.h>
#include <string>
#include <memory>

namespace node_pdfium {
    using Unique_HDC =
        std::unique_ptr<std::remove_pointer<HDC>::type, ReleaseDC>
    Unique_HDC GetPrinterDC(const v8::Local<v8::Value>& printerName);
}

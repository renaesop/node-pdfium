#include "printer_win.h"

Unique_HDC node_pdfium::GetPrinterDC(const v8::Local<v8::Value>& printerName) {
    const v8::Local<v8::String> printerNameV8Str = printerName->ToString();


    auto printerDC = ::CreateDCW(L"WINSPOOL", static_cast<LPCWSTR> (*v8::String::Value(printerNameV8Str)), NULL, NULL);

    if (!printerDC) {
        return -1;
    }
    return Unique_HDC(printerDC);
}
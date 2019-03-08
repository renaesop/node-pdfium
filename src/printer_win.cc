#include "printer_win.h"
#include <iostream>

namespace node_pdfium
{
Unique_HDC GetPrinterDC(const v8::Local<v8::Value> &printerName)
{
    const v8::Local<v8::String> printerNameV8Str = printerName->ToString();

    Unique_HDC printerDC(::CreateDCW(L"WINSPOOL", reinterpret_cast<LPCWSTR>(*v8::String::Value(printerNameV8Str)), NULL, NULL));

    return printerDC;
}
} // namespace node_pdfium
#include "inc.h"

#include "printer_win.h"
#include "pdfium_option.h"
#include "pdfium_imp.h"

#include <iostream>

#define CHECK_STRING(name)                     \
    auto err##name = checkString(name, #name); \
    if (err##name)                             \
    {                                          \
        return;                      \
    }

namespace node_pdfium
{
bool checkString(const v8::Local<v8::Value> &arg, const std::string &name)
{
    v8::Isolate *isolate = v8::Isolate::GetCurrent();
    if (!arg->IsString())
    {
        std::stringstream ss;
        ss << "Wrong type of" << name;
        std::string s = ss.str();
        isolate->ThrowException(v8::Exception::TypeError(Nan::New<v8::String>(s.c_str()).ToLocalChecked()));
        return false;
    }
    return true;
}

void PrintPDF(const Nan::FunctionCallbackInfo<v8::Value> &args)
{
    v8::Isolate *isolate = args.GetIsolate();
    const v8::Local<v8::Value> &printerName = args[0];
    const v8::Local<v8::Value> &filePath = args[1];
    const v8::Local<v8::Value> &v8_options = args[2];

    CHECK_STRING(printerName)

    CHECK_STRING(filePath)


    Unique_HDC printer_dc = std::move(GetPrinterDC(printerName));


    if (printer_dc == nullptr)
    {
        isolate->ThrowException(v8::Exception::TypeError(
            Nan::New<v8::String>("Cannot initial device context for the printer")
                .ToLocalChecked()));
        return;
    }

    std::unique_ptr<PdfiumOption> options(V8OptionToStruct(v8_options));
    // @TODO
    auto filePathStr = Nan::To<v8::String>(printerName).ToLocalChecked();
    auto doc = std::make_unique<PDFDocument>(reinterpret_cast<LPCWSTR>(*v8::String::Value(filePathStr)));

    doc->LoadDocument();

    doc->PrintDocument(printer_dc.get(), *options);
}

NAN_MODULE_INIT(Init)
{
    FPDF_InitLibrary();
    Nan::Set(target, Nan::New<v8::String>("printPDF").ToLocalChecked(),
             Nan::GetFunction(Nan::New<v8::FunctionTemplate>(PrintPDF)).ToLocalChecked());
}

NODE_MODULE(node_pdfium, Init)

} // namespace node_pdfium
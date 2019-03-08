#include <nan.h>
#include <fpdfview.h>
#include <memory>

#include "printer_win.h"
#include "pdfium_option.h"
#include "pdfium_imp.h"
// @TODO new thread?
#define CHECK_STRING(name) auto checked = checkString(#name);\
    if (checked) {return checked}

namespace node_pdfium
{
static v8::Local<v8::Value> checkString(const v8::Local<v8::Value> &arg, const std::string &name)
{
    if (!arg.IsString())
    {
        return isolate->ThrowException(v8::Exception::TypeError(
            v8::String::NewFromUtf8(isolate,
                                    std::string('') + "Wrong type of " + name)
                .ToLocalChecked()));
    }
    return null;
}

void PrintPDF(const FunctionCallbackInfo<Value> &args)
{
    v8::Isolate *isolate = args.GetIsolate();
    const v8::Local<v8::Value> &printerName = args[0];
    const v8::Local<v8::Value> &filePath = args[1];
    const v8::Local<v8::Value> &v8_options = args[2];

    CHECK_STRING(printerName);
    CHECK_STRING(filePath);
    
    Unique_HDC printer_dc = GetPrinterDC(printerName);
    if (HDC == -1)
    {
        return isolate->ThrowException(v8::Exception::TypeError(
            v8::String::NewFromUtf8(isolate,
                                    "Cannot initial device context for the printer")
                .ToLocalChecked()));
    }

    std::unique_ptr<PdfiumOption> options(V8OptionToStruct(v8_options));
    // @TODO
    auto doc = std::make_unique<PDFDocument>(filePath);

    doc->LoadDocument();

    doc->PrintDocument(printer_dc.get(), *options);
}

NAN_MODULE_INIT(Init)
{
    Nan::Set(target, Nan::New<v8::String>("printPDF").ToLocalChecked(),
             Nan::GetFunction(Nan::New<v8::FunctionTemplate>(PrintPDF)).ToLocalChecked());
}

NODE_MODULE(node_pdfium, Init)

} // namespace node_pdfium
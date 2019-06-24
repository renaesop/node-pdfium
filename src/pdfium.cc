#include "inc.h"

#include "printer_win.h"
#include "pdfium_option.h"
#include "pdfium_imp.h"

#define CHECK_STRING(name)                     \
    auto err##name = checkString(name, #name); \
    if (!err##name)                            \
    {                                          \
        return;                                \
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

void checkError(v8::Isolate* isolate, const std::wstring &failure, const std::wstring &arg)
{
    unsigned long err = FPDF_GetLastError();

    std::wstringstream ss;
    ss << "Fail to " << failure << " , for the " << arg << std::endl;
    ss << "Rason: ";
    switch (err)
    {
    case FPDF_ERR_SUCCESS:
        ss << "Success";
        break;
    case FPDF_ERR_UNKNOWN:
        ss << "Unknown error";
        break;
    case FPDF_ERR_FILE:
        ss << "File not found or could not be opened";
        break;
    case FPDF_ERR_FORMAT:
        ss << "File not in PDF format or corrupted";
        break;
    case FPDF_ERR_PASSWORD:
        ss << "Password required or incorrect password";
        break;
    case FPDF_ERR_SECURITY:
        ss << "Unsupported security scheme";
        break;
    case FPDF_ERR_PAGE:
        ss << "Page not found or content error";
        break;
    default:
        ss << "Unknown error " << err;
    }
    auto s = ss.str();
    isolate->ThrowException(v8::Exception::TypeError(
        Nan::New<v8::String>(reinterpret_cast<const uint16_t *>(s.c_str()))
            .ToLocalChecked()));
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
        checkError(isolate, L" initiate device context for the printer", L"");
        return;
    }

    std::unique_ptr<PdfiumOption> options(V8OptionToStruct(v8_options));
    // @TODO
    auto filePathStr = Nan::To<v8::String>(filePath).ToLocalChecked();
    v8::String::Value utf16Val(v8::Isolate::GetCurrent(), filePathStr);
    std::wstring fileWstr(reinterpret_cast<wchar_t *>(*utf16Val), utf16Val.length());
    auto doc = std::make_unique<PDFDocument>(fileWstr.c_str());

    if (!doc->LoadDocument())
    {
        checkError(isolate, L"load the document", fileWstr);
        return;
    }

    try
    {
        doc->PrintDocument(printer_dc.get(), *options);
    }
    catch (const std::exception &e)
    {
     checkError(isolate, L"print the given document", fileWstr);
        return;
    }
}

NAN_MODULE_INIT(Init)
{
    FPDF_InitLibrary();
    Nan::Set(target, Nan::New<v8::String>("printPDF").ToLocalChecked(),
             Nan::GetFunction(Nan::New<v8::FunctionTemplate>(PrintPDF)).ToLocalChecked());
}

NODE_MODULE(node_pdfium, Init)

} // namespace node_pdfium
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
bool checkString(const Napi::Value &arg, const std::string &name)
{
    if (!arg.IsString())
    {
        std::stringstream ss;
        ss << "Wrong type of" << name;
        std::string s = ss.str();
        Napi::Error::New(env, s).ThrowAsJavaScriptException();
        return false;
    }
    return true;
}

void checkError(Napi::Env &env, const std::wstring &failure, const std::wstring &arg)
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
    std::string s = ss.str();
    Napi::Error::New(env, s).ThrowAsJavaScriptException();
}

void PrintPDF(const Napi::CallbackInfo&args)
{
    Napi::Env &env = args.Env();
    const Napi::Value &printerName = args[0];
    const Napi::Value &filePath = args[1];
    const Napi::Value &v8_options = args[2];

    CHECK_STRING(printerName)

    CHECK_STRING(filePath)

    Unique_HDC printer_dc = std::move(GetPrinterDC(printerName));

    if (printer_dc == nullptr)
    {
        checkError(env, L" initiate device context for the printer", L"");
        return;
    }

    std::unique_ptr<PdfiumOption> options(V8OptionToStruct(v8_options));
    // @TODO
    auto utf16Val = filePath.As<Napi::String>().Utf16Value();
    auto fileWstr = reinterpret_cast<LPCWSTR>(utf16Val.c_str());
    auto doc = std::make_unique<PDFDocument>(fileWstr);

    if (!doc->LoadDocument())
    {
        checkError(env, L"load the document", fileWstr);
        return;
    }

    try
    {
        doc->PrintDocument(printer_dc.get(), *options);
    }
    catch (const std::exception &e)
    {
     checkError(env, L"print the given document", fileWstr);
        return;
    }
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    FPDF_InitLibrary();
    exports.Set(Napi::String::New(env, "printPDF"),
            Napi::Function::New(env, PrintPDF));
    return exports;
}

NODE_API_MODULE(node_pdfium, Init)

} // namespace node_pdfium
#include <fpdfview.h>
#include <cpp/fpdf_scopers.h>
#include <memory>
#include <map>
#include <vector>

#include "pdfium_option.h"

// #include <windows.h>
namespace node_pdfium {
    class PDFDocument {
    public:
        PDFDocument(std::wstring&& filename);
        bool LoadDocument();

        int32_t PrintDocument(HDC dc, const PdfiumOption& options);
    private:
        int32_t PrintSinglePage();
        std::wstring filename;
        ScopedFPDFDocument doc;    
        std::map<int, ScopedFPDFPage> loaded_pages;
        int32_t printPage(HDC dc, int32_t index, int32_t width, int32_t height);
        FPDF_PAGE getPage(const FPDF_DOCUMENT& doc, int32_t index);
    }
    
}



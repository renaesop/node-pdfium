#ifndef PDFIUM_IMP
#define PDFIUM_IMP

#include "inc.h"
#include "pdfium_option.h"

// #include <windows.h>
namespace node_pdfium
{
class PDFDocument
{
  public:
    PDFDocument(std::wstring &&filename);
    bool LoadDocument();

    void PrintDocument(HDC dc, const PdfiumOption &options);

  private:
    void PrintSinglePage();
    std::wstring filename;
    ScopedFPDFDocument doc;
    std::map<int, ScopedFPDFPage> loaded_pages;
    void printPage(HDC dc, int32_t index, int32_t width, int32_t height);
    FPDF_PAGE getPage(const FPDF_DOCUMENT &doc, int32_t index);
};
}; // namespace node_pdfium

#endif

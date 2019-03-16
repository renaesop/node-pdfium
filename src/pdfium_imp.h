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
  std::vector<char> file_content;
  void printPage(HDC dc, int32_t index, int32_t width, int32_t height, float ratio);
  FPDF_PAGE getPage(const FPDF_DOCUMENT &doc, int32_t index);
};

class PrinterDocumentJob
{
public:
  PrinterDocumentJob(HDC d, LPCWSTR filename) : dc(d)
  {
    ::DOCINFOW docInfo{sizeof(docInfo), const_cast<LPWSTR>(filename), NULL, NULL, 0};
    // @TODO why error????
    job = ::StartDocW(dc, &docInfo);
    if (job <= 0)
      throw;
  };
  ~PrinterDocumentJob()
  {
    ::EndDoc(dc);
  };

private:
  HDC dc;
  int job;
};

class PrinterPageJob
{
public:
  PrinterPageJob(HDC d) : dc(d)
  {
    auto res = ::StartPage(dc);
    if (!res)
      throw;
  };
  ~PrinterPageJob()
  {
    ::EndPage(dc);
  };

private:
  HDC dc;
};
}; // namespace node_pdfium

#endif

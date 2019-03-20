#include "pdfium_imp.h"
#include <clocale>
#include <cstdlib>

node_pdfium::PDFDocument::PDFDocument(std::wstring &&f) : filename(f)
{
}

bool node_pdfium::PDFDocument::LoadDocument()
{
    std::ifstream pdfStream = std::ifstream(filename, std::ifstream::binary | std::ifstream::in);
    file_content.insert(file_content.end(), std::istreambuf_iterator<char>(pdfStream), std::istreambuf_iterator<char>());
    auto pdf_pointer = FPDF_LoadMemDocument(file_content.data(), (int)file_content.size(), nullptr);

    if (!pdf_pointer)
    {
        return false;
    }
    doc.reset(pdf_pointer);
    return true;
}

void node_pdfium::PDFDocument::PrintDocument(HDC dc, const PdfiumOption &options)
{
    PrinterDocumentJob djob(dc, filename.c_str());

    auto pageCount = FPDF_GetPageCount(doc.get());

    auto width = options.width;
    auto height = options.height;

    for (int16_t i = 0; i < options.copies; ++i)
    {
        if (std::size(options.page_list) > 0)
        {
            for (auto pair : options.page_list)
            {
                for (auto j = pair.first; j < pair.second + 1; ++j)
                {
                    printPage(dc, j, width, height, options.dpi);
                }
            }
        }
        else
        {
            for (auto j = 0; j < pageCount; j++)
            {
                printPage(dc, j, width, height, options.dpi);
            }
        }
    }
}

void node_pdfium::PDFDocument::printPage(HDC dc,
                                         int32_t index, int32_t width, int32_t height, float dpiRatio)
{
    PrinterPageJob pJob(dc);
    auto page = getPage(doc.get(), index);

    if (!page)
    {
        return;
    }
    if (!width)
    {
        width = static_cast<int32_t>(FPDF_GetPageWidth(page) * dpiRatio);
    }
    if (!height)
    {
        height = static_cast<int32_t>(FPDF_GetPageHeight(page) * dpiRatio);
    }
    HRGN rgn = CreateRectRgn(0, 0, width, height);

    ::SelectClipRgn(dc, rgn);
    ::DeleteObject(rgn);

    ::SelectObject(dc, ::GetStockObject(NULL_PEN));
    ::SelectObject(dc, ::GetStockObject(WHITE_BRUSH));
    // If a PS_NULL pen is used, the dimensions of the rectangle are 1 pixel less.
    ::Rectangle(dc, 0, 0, width + 1, height + 1);
    return ::FPDF_RenderPage(dc, page, 0, 0, width, height, 0,
                             FPDF_ANNOT | FPDF_PRINTING | FPDF_NO_CATCH);
}

FPDF_PAGE node_pdfium::PDFDocument::getPage(const FPDF_DOCUMENT &doc, int32_t index)
{
    auto iter = loaded_pages.find(index);
    if (iter != loaded_pages.end())
        return iter->second.get();

    ScopedFPDFPage page(FPDF_LoadPage(doc, index));
    if (!page)
        return nullptr;

    // Mark the page as loaded first to prevent infinite recursion.
    FPDF_PAGE page_ptr = page.get();
    loaded_pages[index] = std::move(page);
    return page_ptr;
}


 
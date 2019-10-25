#include "pdfium_imp.h"
#include <clocale>
#include <cstdlib>
#include <algorithm>

#include <iostream>


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

void setBitmapToBlackWhite(char* buffer, int width, int height, int stride) {
    
    std::cout << "?????" << "width: " << width << "height: " << height << "stride: " << stride  << std::endl;
    auto total_size = stride * height;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < static_cast<int>(stride / 4) && j < width; ++j) {
            auto base_offset = i * stride + j * 4;
            for (int k = 0; k < 3; ++k) {
                if (buffer[base_offset + k] > '\168')
                {
                    buffer[base_offset + k] = '\255';
                }
                else
                {
                    buffer[base_offset + k] = '\0';
                }   
            }
            buffer[base_offset + 3] = '\255'; 
        }
    }
}

void node_pdfium::PDFDocument::setImageToBalckWhite(FPDF_PAGE page)
{
    auto count = FPDFPage_CountObjects(page);
    for (int i = 0; i < count; ++i)
    {
        auto pdf_object = ::FPDFPage_GetObject(page, i);
        if (FPDFPageObj_GetType(pdf_object) == FPDF_PAGEOBJ_IMAGE)
        {
            auto image_bitmap = ::FPDFImageObj_GetBitmap(pdf_object);
            auto buffer = ::FPDFBitmap_GetBuffer(image_bitmap);
            std::cout << "buffer; count: " << count << "i: " << i << std::endl;
            setBitmapToBlackWhite(static_cast<char*>(buffer),
                ::FPDFBitmap_GetWidth(image_bitmap),
                ::FPDFBitmap_GetHeight(image_bitmap),
                ::FPDFBitmap_GetStride(image_bitmap)
            );
              std::cout << "buffer1; count: " << count << "i: " << i << std::endl;
            std::vector<FPDF_PAGE> vec;
            for (const auto &pair : loaded_pages)
            {
                vec.push_back(pair.second.get());
            }
              std::cout << "buffer2; count: " << count << "i: " << i << std::endl;
            ::FPDFImageObj_SetBitmap(vec.data(), std::size(vec), pdf_object, image_bitmap);
        }
    }
}

void node_pdfium::PDFDocument::PrintDocument(HDC dc, const PdfiumOption &options)
{
    PrinterDocumentJob djob(dc, filename.c_str());

    auto pageCount = FPDF_GetPageCount(doc.get());

    auto width = options.width;
    auto height = options.height;
    auto blackWhite = !!options.blackWhite;

    for (int16_t i = 0; i < options.copies; ++i)
    {
        if (std::size(options.page_list) > 0)
        {
            for (auto pair : options.page_list)
            {
                for (auto j = pair.first - 1; j < pair.second && j < pageCount; ++j)
                {
                    printPage(dc, j, width, height, options.dpi, blackWhite);
                }
            }
        }
        else
        {
            for (auto j = 0; j < pageCount; j++)
            {
                printPage(dc, j, width, height, options.dpi, blackWhite);
            }
        }
    }
}

void node_pdfium::PDFDocument::printPage(HDC dc,
                                         int32_t index, int32_t width, int32_t height, float dpiRatio, bool blackWhite)
{
    PrinterPageJob pJob(dc);
    auto page = getPage(doc.get(), index);
    std::cout << "error: " << ::FPDF_GetLastError() << std::endl;
    if (blackWhite) {
        setImageToBalckWhite(page);
    }
    std::cout << "error: " << ::FPDF_GetLastError() << std::endl;

    if (!page)
    {
        return;
    }
    if (!width)
    {
        auto pageWidth = FPDF_GetPageWidth(page);
        width = static_cast<int32_t>(pageWidth * dpiRatio);
    }
    if (!height)
    {
        auto pageHeight = FPDF_GetPageHeight(page);
        height = static_cast<int32_t>(pageHeight * dpiRatio);
    }
    auto mediaWidth = ::GetDeviceCaps(dc, HORZRES);
    auto mediaWHeight = ::GetDeviceCaps(dc, VERTRES);

    HRGN rgn = CreateRectRgn(0, 0, mediaWidth, mediaWHeight);


    ::SelectClipRgn(dc, rgn);
    ::DeleteObject(rgn);

    ::SelectObject(dc, ::GetStockObject(NULL_PEN));
    ::SelectObject(dc, ::GetStockObject(WHITE_BRUSH));

    // If a PS_NULL pen is used, the dimensions of the rectangle are 1 pixel less.
    ::Rectangle(dc, 0, 0, mediaWidth, mediaWHeight);
    return ::FPDF_RenderPage(dc, page, 0, 0, width, height, 0,
                             FPDF_ANNOT | FPDF_PRINTING | FPDF_NO_CATCH | FPDF_RENDER_NO_SMOOTHIMAGE | FPDF_RENDER_NO_SMOOTHTEXT);
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


 
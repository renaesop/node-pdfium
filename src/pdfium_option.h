#ifndef PDFIUM_OPTION
#define PDFIUM_OPTION

#include "inc.h"

namespace node_pdfium {
    struct PdfiumOption {
        PdfiumOption() = default;

        float dpi = 324;
        int32_t copies = 1;
        int32_t width = 0;
        int32_t height = 0;
        bool fit = false;
        std::vector<std::pair<int32_t, int32_t>> page_list;
    };

    std::unique_ptr<PdfiumOption> V8OptionToStruct(const v8::Local<v8::Value>& options);
};
#endif
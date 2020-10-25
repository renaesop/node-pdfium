#include "pdfium_option.h"

#define INIT_PDFIUM_OPTION(name, type)                                           \
    if (obj->Has(Napi::GetCurrentContext(), Napi:: v8::String::New(env, #name)).ToChecked())            \
    {                                                                      \
        ops->name = Napi::To< type>(                                    \
            (obj).Get(Napi:: v8::String::New(env, #name))) \
                        ;                                    \
    }

namespace node_pdfium
{
std::unique_ptr<PdfiumOption> V8OptionToStruct(const Napi::Value &options)
{
    auto ops = std::make_unique<PdfiumOption>();
    if (options.IsObject())
    {
        auto obj = options->ToObject(Napi::GetCurrentContext());

        INIT_PDFIUM_OPTION(dpi, int32_t);
        INIT_PDFIUM_OPTION(copies, int32_t);
        INIT_PDFIUM_OPTION(width, int32_t);
        INIT_PDFIUM_OPTION(height, int32_t);

        ops->dpi = ops->dpi / 72;

        if (obj->Has(Napi::GetCurrentContext(), Napi::String::New(env, "pageList")).ToChecked())
        {
            auto o = (obj).Get(Napi::String::New(env, "pageList")
                                  )
                         ;
            auto arr = o.As<Napi::Array>();
            for (unsigned int i = 0; i < arr->Length(); ++i)
            {
                const auto &item = (arr).Get(i.As<Napi::Array>());
                auto pair = std::make_pair(
                    (arr).Get(0.As<Napi::Number>().Int32Value()),
                    (arr).Get(1.As<Napi::Number>().Int32Value()));
                ops->page_list.push_back(std::move(pair));
            }
        }
    }

    return ops;
}
} // namespace node_pdfium

#include "pdfium_option.h"

#define INIT_PDFIUM_OPTION(name)                                           \
    if (obj->Has(Nan::New< v8::String>(#name).ToLocalChecked()))            \
    {                                                                      \
        ops->name = Nan::To< int32_t>(                                    \
            Nan::Get(obj, Nan::New< v8::String>(#name).ToLocalChecked()).ToLocalChecked()) \
                        .FromJust();                                    \
    }

namespace node_pdfium
{
std::unique_ptr<PdfiumOption> V8OptionToStruct(const v8::Local<v8::Value> &options)
{
    auto ops = std::make_unique<PdfiumOption>();
    if (!options->IsObject())
    {
        auto obj = options->ToObject();

        INIT_PDFIUM_OPTION(dpi);
        INIT_PDFIUM_OPTION(copies);
        INIT_PDFIUM_OPTION(width);
        INIT_PDFIUM_OPTION(height);

        if (obj->Has(Nan::New<v8::String>("pageList").ToLocalChecked()))
        {
            auto o = Nan::Get(obj,
                              Nan::New<v8::String>("pageList")
                                  .ToLocalChecked())
                         .ToLocalChecked();
            auto arr = v8::Local<v8::Array>::Cast(o);
            for (unsigned int i = 0; i < arr->Length(); ++i)
            {
                const auto &item = v8::Local<v8::Array>::Cast(Nan::Get(arr, i).ToLocalChecked());
                auto pair = std::make_pair(
                    Nan::To<int32_t>(Nan::Get(arr, 0).ToLocalChecked()).FromJust(),
                    Nan::To<int32_t>(Nan::Get(arr, 1).ToLocalChecked()).FromJust());
                ops->page_list.push_back(std::move(pair));
            }
        }
    }

    return ops;
}
} // namespace node_pdfium

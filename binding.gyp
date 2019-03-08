{
    "targets": [
        {
            "target_name": "pdfium",
            "sources": [
                "pdfium.cc",
                "printer_win.cc",
                "pdfium_option.cc",
                "pdfium_imp.cc"
            ],
            "include_dirs": [
                "<!(node -e \"require('nan')\")"
            ],
            "variables": {
                "GS4JS_HOME%": "<!(node gs4js-env-home.js)",
                "GS4JS_LIB%": "pdfium.lib",
                "GS4JS_DLL%": "pdfium.dll",
                "conditions": [
                    ['"<!(node gs4js-env-home.js)" == ""', {
                        "GS4JS_HOME%": "C:/Downloads/pdfium"
                    }]
                ]
            },
            "libraries": ["<(module_root_dir)/build/Release/<(GS4JS_LIB)"],
            "copies": [
                {
                    "destination": "<(module_root_dir)/build/Release",
                    "files": ["<(GS4JS_HOME)/<(GS4JS_DLL)", "<(GS4JS_HOME)/<(GS4JS_LIB)"]
                }
            ]
        }
    ]
}

{
    "targets": [
        {
            "target_name": "pdfium",
            "sources": [
                "src/pdfium_option.cc",
                "src/printer_win.cc",
                "src/pdfium_imp.cc",
                 "src/pdfium.cc"
            ],
            "include_dirs": [
                "<!(node -e \"require('nan')\")",
                "src/include"
            ],
            "variables": {
                "GS4JS_HOME%": "<!(node pdfium-env-home.js)",
                "GS4JS_LIB%": "pdfium.dll.lib",
                "GS4JS_DLL%": "pdfium.dll",
                "conditions": [
                    ['"<!(node pdfium-env-home.js)" == ""', {
                        "GS4JS_HOME%": "C:/Downloads/pdfium"
                    }]
                ]
            },
            "libraries": ["<(module_root_dir)/build/Release/<(GS4JS_LIB)"],
            "copies": [
                {
                    "destination": "<(module_root_dir)/build/Release",
                    "files": ["<(GS4JS_HOME)/bin/<(GS4JS_DLL)", "<(GS4JS_HOME)/lib/<(GS4JS_LIB)"]
                }
            ]
        }
    ]
}

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
                "PDFIUM_HOME%": "<!(node pdfium-env-home.js)",
                "PDFIUM_LIB%": "pdfium.dll.lib",
                "PDFIUM_DLL%": "pdfium.dll",
                "conditions": [
                    ['"<!(node pdfium-env-home.js)" == ""', {
                        "PDFIUM_HOME%": "C:/Downloads/pdfium"
                    }]
                ]
            },
            "libraries": ["<(module_root_dir)/build/Release/<(PDFIUM_LIB)"],
            "copies": [
                {
                    "destination": "<(module_root_dir)/build/Release",
                    "files": ["<(PDFIUM_HOME)/bin/<(PDFIUM_DLL)", "<(PDFIUM_HOME)/lib/<(PDFIUM_LIB)"]
                }
            ]
        }
    ]
}

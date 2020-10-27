{
    "targets": [
        {
            "target_name": "pdfium",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "xcode_settings": { "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
        "CLANG_CXX_LIBRARY": "libc++",
        "MACOSX_DEPLOYMENT_TARGET": "10.7",
      },
      "msvs_settings": {
        "VCCLCompilerTool": { "ExceptionHandling": 1 },
      },
            "sources": [
                "src/pdfium_option.cc",
                "src/printer_win.cc",
                "src/pdfium_imp.cc",
                 "src/pdfium.cc"
            ],
            "include_dirs": [
              "<!(node -p \"require('node-addon-api').include_dir\")",
                "src/include"
            ],
            "variables": {
                "PDFIUM_HOME%": "<!(npx -q cross-env TARGET_ARCH=<(target_arch) node pdfium-env-home.js)",
                "PDFIUM_LIB%": "pdfium.dll.lib",
                "PDFIUM_DLL%": "pdfium.dll",
                "conditions": [
                    ['"<!(npx -q cross-env TARGET_ARCH=<(target_arch) node pdfium-env-home.js)" == ""', {
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

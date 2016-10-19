import qbs.base 1.0
import qbs.Process

QtApplication {
    cpp.cxxFlags: {
        if (qbs.toolchain.contains('gcc') || qbs.toolchain.contains("mingw")) {
            return '-std=c++11';
        }
    }

    name: "aotc"
    //destinationDirectory: aotc_bin_path

    Depends { name: "cpp" }
    Depends { name: "Qt"; submodules: [ 'core', 'gui', "widgets" ] }

    consoleApplication: false
    cpp.includePaths: [ '.']

    cpp.defines: base.concat([
        'VERSION_MAJOR="' + app_version_major + '"',
        'VERSION_MINOR="' + app_version_minor + '"',
    ])

    files: [
        "globals.h",
        "main.cpp",
        "ui/*.h",
        "ui/*.cpp",
        "ui/*.ui",
    ]

    Group {
        name: "Resources"
//        prefix: "../"
        files: [
            "res.qrc",
        ]
    }

    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: aotc_bin_path
    }
}

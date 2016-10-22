import qbs.base
import qbs.Process

Project {
    name: "Always on top clock"
    property string app_target: "aotc"

    property int app_version_major: 1
    property int app_version_minor: 0

    property string aotc_bin_path: "bin"

    references: [
        "src/app.qbs",
    ]
}

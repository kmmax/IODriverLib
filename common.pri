# Common parameters for all files


PROJECT_ROOT_PATH = $${PWD}

win32: {
    contains(QT_ARCH, i386) {
    # Windows x86 (32bit)
        OS_SUFFIX = win32
    }
    else {
        # Windows x86_64 (64 bit)
        OS_SUFFIX = win64
    }
}
linux-g++: OS_SUFFIX = linux
message("OS_SUFFIX="$$OS_SUFFIX)

# Path for QtDesinger plugins
#MX_INSTALL_PLUGINS = /home/max/Qt5.11.1-1/Tools/QtCreator/lib/Qt/plugins
MX_INSTALL_PLUGINS = "C:/Qt/Qt5.11.2/Tools/QtCreator/bin/plugins"

QT -= gui

CONFIG += c++14 console thread
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp
		
DESTDIR = build

unix:!macx: LIBS += -L$$PWD/../../../build/src/lib/ -lIODriver

INCLUDEPATH += $$PWD/../../../src/IODriver
DEPENDPATH += $$PWD/../../../src/IODriver

#unix:!macx: LIBS += -L$$PWD/../../../bin/ -lIODriver
#unix:!macx: LIBS += -L$$PWD/../../../bin/ -lmodbus

win32: LIBS += -L$$PWD/../../../build/src/lib/ -llibIODriver.dll
#win32: LIBS += -L$$PWD/../../../bin/windows_mingw_32/ -llibmodbus.dll
#message($$PWD)


#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../bin/windows/mingw53_32/ -llibIODriver.dll
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../bin/windows/mingw53_32/ -llibIODriver.dlld

#INCLUDEPATH += $$PWD/../../../bin/windows/mingw53_32
#DEPENDPATH += $$PWD/../../../bin/windows/mingw53_32

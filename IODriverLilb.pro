include(common.pri)

QT -= gui

TEMPLATE = lib
DEFINES += LIBMODBUS_LIBRARY
CONFIG += c++11

TARGET = libIODriver

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/import/libmodbus/includes

SOURCES += \
    src/IODriver/abstractdriver.cpp \
    src/IODriver/iodriverlib.cpp \
    src/IODriver/memorydriver.cpp \
    src/IODriver/modbusdriver.cpp \
    src/IODriver/utilites.cpp

HEADERS += \
    src/IODriver/abstractdriver.h \
    src/IODriver/idriver.h \
    src/IODriver/iodriverlib.h \
    src/IODriver/log.h \
    src/IODriver/memorydriver.h \
    src/IODriver/modbusdriver.h \
    src/IODriver/utilites.h \
    src/IODriver/version.h

#LIBS += -lws2_32


#LIBS += -L$$PWD/import/libmodbus/lib/$$OS_SUFFIX/msvs/ -llibmodbus
LIBS += -L$$PWD/import/libmodbus/lib/$$OS_SUFFIX/ -llibmodbus
message($$LIBS)

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

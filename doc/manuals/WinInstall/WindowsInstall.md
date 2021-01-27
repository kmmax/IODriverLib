## Installation in Windows (MinGW)

You will only need to install automake, autoconf, libtool and a C compiler (gcc)

## 1 Installing by script

add...

## 2 Installing manually (CMake)
2.1. Качаем исходниками из репозитория git:

~~~bash
$ git clone https://github.com/kmmax/IODriverLib.git && cd IODriverLib
~~~

2.2. Копируем зависимости (libmodbu) в папки
файлы библиотек:
+ **import/libmodbus/lib/win32** - для migw 32 bit;
+ **import/libmodbus/lib/win64** - для migw 64 bit;
+ **import/libmodbus/lib/unix64** - для gcc 64 bit;

заголовочные файлы (в папку **import/libmodbus/includes**):
+ **modbus.h**
+ **modbus-private.h**
+ **modbus-rtu.h**
+ **modbus-rtu-private.h**
+ **modbus-tcp.h**
+ **modbus-tcp-private.h**
+ **modbus-version.h**

2.3. Create and go to the **build** directory
~~~cmake
$ mkdir build && cd build
~~~
2.4. For building library execute the command:
~~~cmake
$ cmake .. -G "Unix Makefiles" mingw32-make.exe &&  mingw32-make.exe installk
~~~
or
~~~cmake
$ export TOOL=c\://Qt/Qt5.11.2/Tools/mingw810_64/bin
$ cmake .. -G "Unix Makefiles" -DCMAKE_CXX_COMPILER=$TOOL"/g++.exe" -DCMAKE_MAKE_PROGRAM=$TOOL"/mingw32-make.exe" -DAPP_OS="win64" && $TOOL"/mingw32-make.exe" && $TOOL"/mingw32-make.exe" install
~~~

options:
+ TEST=ON/OFF - Build unit tests (if need)
+ APP_OS="win32"/"win64"/"unix64" - for linking libmodbus library




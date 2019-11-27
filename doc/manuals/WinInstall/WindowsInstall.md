## Installation in Windows (MinGW)

You will only need to install automake, autoconf, libtool and a C compiler (gcc)

## 1 Installing by script

add...

## 2 Installing manually

1. Create and go to the **build** directory
~~~cmake
$ mkdir build && cd build
~~~
2. For building library execute the command:
~~~cmake
$ cmake .. -G "Unix Makefiles" mingw32-make.exe &&  mingw32-make.exe install
~~~
3. Build unit tests (if need). (Option **-DTEST=ON** enables building tests)
~~~cmake
$ cmake .. -G "Unix Makefiles" -DTEST=ON &&  mingw32-make.exe &&  mingw32-make.exe install
~~~
4. If you want to use another compiler you should specify it directly (For example MinGW from Qt)
~~~cmake
$ cmake .. -G "Unix Makefiles" -DCMAKE_CXX_COMPILER="C:/Qt/Qt5.11.2/Tools/mingw530_32/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="C:/Qt/Qt5.11.2/Tools/mingw530_32/bin/mingw32-make.exe" && mingw32-make.exe &&  mingw32-make.exe install
~~~
5. Building unit tests
~~~cmake
$ cmake .. -G "Unix Makefiles" -DCMAKE_CXX_COMPILER="C:/Qt/Qt5.11.2/Tools/mingw530_32/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="C:/Qt/Qt5.11.2/Tools/mingw530_32/bin/mingw32-make.exe" -DTEST=ON &&  mingw32-make.exe &&  mingw32-make.exe install
~~~




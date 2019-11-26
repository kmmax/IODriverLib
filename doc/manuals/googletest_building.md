## How to build googletest

1. download **googletest** into **/import/googletest/src**
~~~cmake
$ git clone https://github.com/google/googletest.git
~~~

2. Create build here (in src folder) and go there:
~~~cmake
$ mkdir build && cd build
~~~

3. Building shared library (MinGW Qt5.11.2)
~~~cmake
$ cmake ../googletest/ -G "Unix Makefiles" -DBUILD_SHARED_LIBS=ON -DCMAKE_CXX_COMPILER="C:/Qt/Qt5.11.2/Tools/mingw530_32/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="C:/Qt/Qt5.11.2/Tools/mingw530_32/bin/mingw32-make.exe" && mingw32-make.exe
~~~

4. libraries will be found in folders:
- **build/lib**:
>- libgmock.dll.a
>- libgmock_main.dll.a
>- libgtest.dll.a
>- libgtest_main.dll.a
- **build/bin**:
>- libgmock.dll
>- libgmock_main.dll
>- libgtest.dll
>- libgtest_main.dll

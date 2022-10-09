#!/bin/bash


# defines
ROOT=$PWD


# === libmodbus ===
# clone
cd $ROOT/import/libmodbus/src/
echo "-- Changin directory: "$PWD
if [ -d $ROOT/import/libmodbus/src/libmodbus ]
then
	echo "-- Removint old libmodbus"
	rm -rf $ROOT/import/libmodbus/src/libmodbus
fi
git clone https://github.com/stephane/libmodbus.git

# build
cd libmodbus
echo "-- Changin directory: "$PWD
./autogen.sh
./configure
make

# copy files
echo "-- Coping files:"
cp -R $ROOT/import/libmodbus/src/libmodbus/src/.libs/*.so* $ROOT/import/libmodbus/lib/unix64/
cp -R $ROOT/import/libmodbus/src/libmodbus/src/*.h $ROOT/import/libmodbus/includes/


# === GTest ===
echo "=== Building googletest ==="
cd $ROOT/import/googletest/src/
if [ -d $ROOT/import/googletest/src/googletest ]
then
	echo "-- Removint old googletest"
	rm -rf $ROOT/import/googletest/src/googletest
fi
git clone https://github.com/google/googletest.git
mkdir build && cd build
cmake ../googletest/ -G "Unix Makefiles" -DBUILD_SHARED_LIBS=ON && make 
cp $ROOT/import/googletest/src/build/lib/* $ROOT/import/googletest/lib/unix64/
cp $ROOT/import/googletest/src/googletest/googletest/include/* -R $ROOT/import/googletest/includes/


# === IODriverLib ===
if [ -d $ROOT/build ]
then
	echo "-- Removint old googletest"
	rm -rf $ROOT/build
fi
mkdir $ROOT/build && cd $ROOT/build
cmake .. -DTEST=OFF && make && make install
echo "-- Done"

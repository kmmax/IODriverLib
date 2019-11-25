# Platform Variables
# UNIX 	  : is TRUE on all UNIX-like OS's, including Apple OS X and CygWin
# WiN32	  : is TRUE on Windows. Prior to 2.8.4 this included CygWin
# APPLE	  : is TRUE on Apple systems. Note this does not imply the system in Mac OS X, only that
# 	    APPLE is #defined in C/C++ header files.
# MINGW	  : is TRUE when using the MinGW compiler in Windows
# MSYS	  : is TRUE when using the MSYS developer environment in Windows
# CYGWIN  : is TRUE on Windows when using the CygWin version of cmake



set(THIS_PROJECT_NAME "SRC")
cmake_minimum_required(VERSION 3.10)
project(THIS_PROJECT_NAME LANGUAGES CXX)
message("=== CMakeLists.txt: " ${THIS_PROJECT_NAME} " ====")


set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 11)


# --- IODriver ---
set(MODULE_NAME "IODriver")
# Settings
file(GLOB HEADERS ${DIR_SRC}/${MODULE_NAME}/*.h)	# All headers
file(GLOB SOURCES ${DIR_SRC}/${MODULE_NAME}/*.cpp)	# All sources
file(GLOB EXPORT_HEADERS 							# Only Export headers
	"${DIR_SRC}/${MODULE_NAME}/idriver.h"
	"${DIR_SRC}/${MODULE_NAME}/iodriverlib.h"
	)
file(GLOB IMPORT_LIB
	"${DIR_IMPORT}/libmodbus/lib/win32/*"			# libmodbus
	)
include_directories(
	${DIR_EXPORT}
	${DIR_EXPORT}/includes							# exported includes
	${DIR_IMPORT}/libmodbus/includes				# extern library includesincludes
	)
	
# Building
add_library(${MODULE_NAME} SHARED ${SOURCES})


# Linking libmodbus
find_library(LIB_MODBUS modbus PATH 
	"${DIR_IMPORT}/libmodbus/lib/win32"
	"${DIR_IMPORT}/libmodbus/lib/unix64"
	)
target_link_libraries(${MODULE_NAME} ${LIB_MODBUS})


get_target_property(DIR ${MODULE_NAME} BINARY_DIR)
set(IODRIVER_BINARY_DIR ${DIR} PARENT_SCOPE)


# Installing
install(TARGETS ${MODULE_NAME} DESTINATION ${DIR_EXPORT}/bin)
install(FILES ${EXPORT_HEADERS} DESTINATION ${DIR_EXPORT}/includes)
install(FILES ${IMPORT_LIB} DESTINATION ${DIR_EXPORT}/bin)






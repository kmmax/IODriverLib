# Platform Variables
# UNIX 	  : is TRUE on all UNIX-like OS's, including Apple OS X and CygWin
# WiN32	  : is TRUE on Windows. Prior to 2.8.4 this included CygWin
# APPLE	  : is TRUE on Apple systems. Note this does not imply the system in Mac OS X, only that
# 	    APPLE is #defined in C/C++ header files.
# MINGW	  : is TRUE when using the MinGW compiler in Windows
# MSYS	  : is TRUE when using the MSYS developer environment in Windows
# CYGWIN  : is TRUE on Windows when using the CygWin version of cmake


#include(ExternalProject)
#ExternalProject_Add(googletest
#    	DOWNLOAD_COMMAND git clone https://github.com/google/googletest.git
#    	DOWNLOAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/import/googletest/src
#		SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/import/googletest/src/googletest
#		BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/import/googletest/src/build
#)

add_subdirectory(Test_IODriverLib)
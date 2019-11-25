# Platform Variables
# UNIX 	  : is TRUE on all UNIX-like OS's, including Apple OS X and CygWin
# WiN32	  : is TRUE on Windows. Prior to 2.8.4 this included CygWin
# APPLE	  : is TRUE on Apple systems. Note this does not imply the system in Mac OS X, only that
# 	    APPLE is #defined in C/C++ header files.
# MINGW	  : is TRUE when using the MinGW compiler in Windows
# MSYS	  : is TRUE when using the MSYS developer environment in Windows
# CYGWIN  : is TRUE on Windows when using the CygWin version of cmake


set(THIS_PROJECT_NAME "Test_IODriver")
message("=== CMakeLists.txt: " ${THIS_PROJECT_NAME} " ====")
cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 11)
project(THIS_PROJECT_NAME LANGUAGES CXX)



include_directories(
	${DIR_EXPORT}/includes
	${DIR_IMPORT}/googletest/includes	
)


#file(GLOB MODBUS_HEADERS ${DIR_IMPORT}/libmodbus/includes/*.h)
#file(GLOB IODRIVER_HEADERS ${DIR_EXPORT}/includes/*.h)


# libmodbus
set(LIB_LIBMODBUS_DIR "libmodbus")
find_library(LIB_MODBUS_LIST modbus PATH
	"${DIR_IMPORT}/${LIB_LIBMODBUS_DIR}/lib/win32"
	"${DIR_IMPORT}/${LIB_LIBMODBUS_DIR}/lib/unix64"
	)
message(STATUS "<-- libmodbus finding -->")


# googletest
set(LIB_GTEST_DIR "googletest")
set(LIB_GOOGLETEST_NAMES gmock gmock_main gtest gtest_main)
foreach(LIB ${LIB_GOOGLETEST_NAMES})
	set(LIB_FOUND "FOUND_LIB-NOTFOUND")
	find_library(LIB_FOUND ${LIB} PATH
		"${DIR_IMPORT}/${LIB_GTEST_DIR}/lib/win32"
		"${DIR_IMPORT}/${LIB_GTEST_DIR}/lib/unix64"
		)
	list(APPEND LIB_GTEST_LIST ${LIB_FOUND})
endforeach(LIB)
#print(LIB_MODBUS_LIST LIB_GTEST_LIST)
message(STATUS "<-- libmodbus finding -->")


# IODriver
find_library(LIB_IODRIVER_LIST IODriver PATH
	"${IODRIVER_BINARY_DIR}/bin"
	"${IODRIVER_BINARY_DIR}/lib"
	)
message(STATUS "<-- IODriver -->")
print(
	IODRIVER_BINARY_DIR
	LIB_MODBUS_LIST
	LIB_GTEST_LIST
	LIB_IODRIVER_LIST
	)


# Test_IODriver
add_executable(${THIS_PROJECT_NAME} main.cpp)
target_link_libraries(${THIS_PROJECT_NAME} 
	#"${CMAKE_CURRENT_SOURCE_DIR}/../lib/libIODriver.dll"
	"${LIB_IODRIVER_LIST}"
	"${LIB_MODBUS_LIST}"
	"${LIB_GTEST_LIST}"
)


get_target_property(DIR1 IODriver BINARY_DIR)
set(IODRIVER_BINARY_DIR ${DIR1})
get_target_property(DIR2 ${THIS_PROJECT_NAME} BINARY_DIR)
set(TEST_IODRIVER_BINARY_DIR ${DIR2})
print(
	IODRIVER_BINARY_DIR 
	TEST_IODRIVER_BINARY_DIR 
	CMAKE_RUNTIME_OUTPUT_DIRECTORY)
message(STATUS "<-- Get target property -->")


add_test(NAME ${THIS_PROJECT_NAME} COMMAND ${THIS_PROJECT_NAME})

# build directory
if(WIN32)
	set(BUILD_BIN_SUFFIX "win32")
elseif(UNIX)
	set(BUILD_BIN_SUFFIX "unix64")
endif()
file(GLOB IODRIVER_BIN_FILES ${IODRIVER_BINARY_DIR}/${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/*)	# All headers
install(FILES ${IODRIVER_BIN_FILES} 
		DESTINATION ${TEST_IODRIVER_BINARY_DIR}/${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(GLOB LIBMODBUS_BIN_FILES
	"${DIR_IMPORT}/libmodbus/lib/${BUILD_BIN_SUFFIX}/*"			# windows
	)	
install(FILES ${LIBMODBUS_BIN_FILES} 
		DESTINATION ${TEST_IODRIVER_BINARY_DIR}/${CMAKE_RUNTIME_OUTPUT_DIRECTORY})	

file(GLOB GTEST_BIN_FILES
	"${DIR_IMPORT}/googletest/lib/${BUILD_BIN_SUFFIX}/*"			# windows
	)	
install(FILES ${GTEST_BIN_FILES} 
		DESTINATION ${TEST_IODRIVER_BINARY_DIR}/${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# export directory
install(TARGETS ${THIS_PROJECT_NAME} DESTINATION ${DIR_EXPORT}/test)


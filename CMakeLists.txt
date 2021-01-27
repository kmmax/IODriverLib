cmake_minimum_required(VERSION 3.10)

# Possible APP_OS values:
# win32 - mingw 32 bit
# win64 - mingw 64 bit
# unix64 - linux 64 bit


# prints variable's context
# prints vars context
function(print)
	message(STATUS ================)
	message(STATUS "Dumping ${ARGC} variables:")
	foreach(VAR_NAME ${ARGN})
		message(STATUS " ${VAR_NAME} == \"${${VAR_NAME}}\"")
	endforeach()
	message(STATUS ================)
endfunction()


set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STADARD 11)


option(TEST "Builds unit tests" OFF)
set(APP_OS "win32" CACHE STRING "Platform")

project(IODriver LANGUAGES CXX)
message("=== CMakeLists.txt: Main ====")


# Global definitions
message("--- Clobal definitions: ---")
set(DIR_SRC ${CMAKE_SOURCE_DIR}/src)
set(DIR_BUILD ${CMAKE_SOURCE_DIR}/build)
set(DIR_IMPORT ${CMAKE_SOURCE_DIR}/import)
set(DIR_EXPORT ${CMAKE_SOURCE_DIR}/export)
set(DIR_TESTS ${CMAKE_SOURCE_DIR}/tests)
set(DIR_BIN ${CMAKE_SOURCE_DIR}/bin)
print(DIR_SRC DIR_BUILD DIR_IMPORT DIR_EXPORT DIR_TESTS DIR_BIN TEST APP_OS)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)

add_subdirectory(src)
if (TEST)
	enable_testing()
	add_subdirectory(tests)
endif()


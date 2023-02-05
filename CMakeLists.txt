cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)
project(snail)
add_definitions("-Wall -g")
include (cmake/utils.cmake)

set(CMAKE_VERBOSE_MSKEFILE ON)
set(CMAEK_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -ggdb -std=c++11 -Wall -Wno-deprecated 
	-Werror -Wno-unused-function -Wno=builtin-macro-redefined")

set(LIB_SRC
    snail/log.cc
	snail/util.cc
	snail/config.cc
	snail/thread.cc
	)


add_library(snail SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(snail)
#add_library(snail_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES (snail_static PROPERTIES OUTPUT_NAME "snail")

set(LIB_LIB
	snail
	pthread
	yaml-cpp
	)


add_executable(test_main tests/test.cc)
force_redefine_file_macro_for_sources(test_main)
add_dependencies(test_main snail)

target_link_libraries(test_main ${LIB_LIB})

add_executable(test_config tests/test_config.cc)
add_dependencies(test_config snail)
force_redefine_file_macro_for_sources(test_config)
target_link_libraries(test_config ${LIB_LIB})

add_executable(test_thread tests/test_thread.cc)
add_dependencies(test_thread snail)
force_redefine_file_macro_for_sources(test_thread)
target_link_libraries(test_thread ${LIB_LIB})



SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)
project(snail)
add_definitions("-Wall -g")
include (cmake/utils.cmake)
include_directories(.)
set(CMAKE_VERBOSE_MSKEFILE ON)
set(CMAEK_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 -Wall -Wno-deprecated 
	-Werror -Wno-unused-function -Wno=builtin-macro-redefined")

set(LIB_SRC
    	snail/log.cc
	snail/util.cc
	snail/config.cc
	snail/thread.cc
	snail/fiber.cc
	snail/scheduler.cc
	snail/iomanager.cc
	snail/timer.cc
	snail/hook.cc
	snail/fd_manager.cc
	snail/address.cc
	snail/socket.cc
	snail/bytearray.cc
	snail/http/http.cc
	snail/http/http11_parser.rl.cc
	snail/http/httpclient_parser.rl.cc
	snail/http/http_parser.cc
	snail/http/http_session.cc
	snail/http/http_server.cc
	snail/http/servlet.cc
	snail/http/http_connection.cc
	snail/tcp_server.cc
	snail/stream.cc
	snail/socket_stream.cc
	snail/uri.rl.cc
)

ragelmaker(snail/http/http11_parser.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/snail/http)
ragelmaker(snail/http/httpclient_parser.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/snail/http)
ragelmaker(snail/uri.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/snail)


add_library(snail SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(snail)
#add_library(snail_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES (snail_static PROPERTIES OUTPUT_NAME "snail")

set(LIB_LIB
	snail
	pthread
	yaml-cpp
	dl
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

add_executable(test_util tests/test_util.cc)
add_dependencies(test_util snail)
force_redefine_file_macro_for_sources(test_util)
target_link_libraries(test_util ${LIB_LIB})

add_executable(test_fiber tests/test_fiber.cc)
add_dependencies(test_fiber snail)
force_redefine_file_macro_for_sources(test_fiber)
target_link_libraries(test_fiber ${LIB_LIB})

add_executable(test_scheduler tests/test_scheduler.cc)
add_dependencies(test_scheduler snail)
force_redefine_file_macro_for_sources(test_scheduler)
target_link_libraries(test_scheduler ${LIB_LIB})


add_executable(test_iomanager tests/test_iomanager.cc)
add_dependencies(test_iomanager snail)
force_redefine_file_macro_for_sources(test_iomanager)
target_link_libraries(test_iomanager ${LIB_LIB})

add_executable(test_hook tests/test_hook.cc)
add_dependencies(test_hook snail)
force_redefine_file_macro_for_sources(test_hook)
target_link_libraries(test_hook ${LIB_LIB})

add_executable(test_address tests/test_address.cc) 
add_dependencies(test_address snail)
force_redefine_file_macro_for_sources(test_address)
target_link_libraries(test_address ${LIB_LIB})

add_executable(test_socket tests/test_socket.cc) 
add_dependencies(test_socket snail)
force_redefine_file_macro_for_sources(test_socket)
target_link_libraries(test_socket ${LIB_LIB})

add_executable(test_bytearray tests/test_bytearray.cc) 
add_dependencies(test_bytearray snail)
force_redefine_file_macro_for_sources(test_bytearray)
target_link_libraries(test_bytearray ${LIB_LIB})

add_executable(test_http tests/test_http.cc) 
add_dependencies(test_http snail)
force_redefine_file_macro_for_sources(test_http)
target_link_libraries(test_http ${LIB_LIB})

add_executable(test_tcp_server tests/test_tcp_server.cc) 
add_dependencies(test_tcp_server snail)
force_redefine_file_macro_for_sources(test_tcp_server)
target_link_libraries(test_tcp_server ${LIB_LIB})

add_executable(test_http_parser tests/test_http_parser.cc) 
add_dependencies(test_http_parser snail)
force_redefine_file_macro_for_sources(test_http_parser)
target_link_libraries(test_http_parser ${LIB_LIB})

add_executable(test_http_server tests/test_http_server.cc) 
add_dependencies(test_http_server snail)
force_redefine_file_macro_for_sources(test_http_server)
target_link_libraries(test_http_server ${LIB_LIB})

add_executable(test_http_connection tests/test_http_connection.cc) 
add_dependencies(test_http_connection snail)
force_redefine_file_macro_for_sources(test_http_connection)
target_link_libraries(test_http_connection ${LIB_LIB})


add_executable(echo_server examples/echo_server.cc) 
add_dependencies(echo_server snail)
force_redefine_file_macro_for_sources(echo_server)
target_link_libraries(echo_server ${LIB_LIB})

snail_add_executable(test_uri "tests/test_uri.cc" snail "${LIB_LIB}")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

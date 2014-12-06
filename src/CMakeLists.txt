cmake_minimum_required( VERSION 2.4)


add_definitions(-DNORMALUNIX -DLINUX)

file( GLOB HEADER_FILES
		*.h)

file( GLOB C_FILES
		*.c)

file( GLOB CXX_FILES
		*.cpp)

add_executable( doom
		${HEADER_FILES}
		${C_FILES}
		${CXX_FILES})

set_target_properties(doom PROPERTIES COMPILE_FLAGS "-g -w -m32 -fpermissive")
set_target_properties(doom PROPERTIES LINK_FLAGS "-m32")

include(FindPkgConfig)

SET(ENV{PKG_CONFIG_LIBDIR} /usr/lib/i386-linux-gnu/pkgconfig)
pkg_search_module(SDL2 REQUIRED sdl2)

list(APPEND ADD_LIB_DIRS ${SDL2_LIBRARY_DIRS})
include(${SDL2_INCLUDE_DIRS})

target_link_libraries( doom
			${SDL2_LIBRARIES}
			nsl
			m)



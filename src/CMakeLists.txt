cmake_minimum_required( VERSION 2.4)

if(NOT WIN32)
	add_definitions(-DNORMALUNIX -DLINUX)
endif(NOT WIN32)

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

set_target_properties(doom PROPERTIES COMPILE_FLAGS "-g -w -m32")
set_target_properties(doom PROPERTIES LINK_FLAGS "-m32")

IF(WIN32)
	set(SDL2_LIBRARY_DIRS "SDL2-NOTFOUND" CACHE PATH "SDL2 Library")
	set(SDL2_INCLUDE_DIRS "SDL2-NOTFOUND" CACHE PATH "SDL2 Includes")
	set(SDL2_LIBARIES SDL2
						SDL2main)
ELSE(WIN32)
	include(FindPkgConfig)

	SET(ENV{PKG_CONFIG_LIBDIR} /usr/lib/i386-linux-gnu/pkgconfig)
	pkg_search_module(SDL2 REQUIRED sdl2)
ENDIF(WIN32)

list(APPEND ADD_LIB_DIRS ${SDL2_LIBRARY_DIRS})
list(APPEND ADD_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS})

target_link_libraries( doom
			${SDL2_LIBRARIES}
			nsl
			m)
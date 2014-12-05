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

target_link_libraries( doom Xext X11 nsl m )		



cmake_minimum_required(VERSION 2.8)

## Use the variable PROJECT_NAME for changing the target name
set( PROJECT_NAME "TRENDS" )
set( TESTGTK_NAME "TESTGTK" )
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
## Set our project name
project(${PROJECT_NAME})
## trends_SOURCE_DIR
## trends_BINARY_DIR
IF (WIN32)
    MESSAGE(STATUS “This\ is\ windows.”)
ELSE (WIN32)
    MESSAGE(STATUS “This\ is\ not\ windows”)
ENDIF (WIN32)
MESSAGE(STATUS "This is PROJECT ROOT PATH:  " ${TRENDS_SOURCE_DIR})
SET(trends_incpath_prefix "${TRENDS_SOURCE_DIR}/solo_opensource_lib")
SET(trends_srcpath_prefix "${TRENDS_SOURCE_DIR}/solo_opensource_src")
SET(trends_buildtool_prefix "${TRENDS_SOURCE_DIR}/../../build_tools")
MESSAGE(STATUS "This is OPENSOURC include file prefix path:  " ${trends_incpath_prefix})

## SET(BUILD_SHARED_LIBS ON)  DEFAULT GEN_LIB IS STATIC
ADD_DEFINITIONS(-DCURL_STATICLIB)
ADD_DEFINITIONS(-DLOG4C_STATICLIB)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -g -Wall -O2 -mms-bitfields")  
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -O2 -mms-bitfields")
##　set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -mwindows")
##  set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -mwindows")
## set(CMAKE_C_FLAGS_DEBUG ”${CMAKE_C_FLAGS} -g -ggdb -pg”)
## set(CMAKE_C_FLAGS_RELEASE ”${CMAKE_C_FLAGS} -s -DNDEBUG”)
INCLUDE_DIRECTORIES(BEFORE 
	"${trends_srcpath_prefix}/curl-7.48.0/dist/include"
	"${trends_srcpath_prefix}/openssl-1.0.2g/dist/include"
	"${trends_srcpath_prefix}/zlib-1.2.8/dist/include"
	"${trends_incpath_prefix}/klib/include"
	"${trends_incpath_prefix}/log4c/include"
	"${trends_buildtool_prefix}/gtk3/include"
	"${trends_buildtool_prefix}/gtk3/include/atk-1.0"
	"${trends_buildtool_prefix}/gtk3/include/cairo"
	"${trends_buildtool_prefix}/gtk3/include/freetype2"
	"${trends_buildtool_prefix}/gtk3/include/gdk-pixbuf-2.0"
	"${trends_buildtool_prefix}/gtk3/include/glib-2.0"
	"${trends_buildtool_prefix}/gtk3/include/gtk-3.0"
	"${trends_buildtool_prefix}/gtk3/include/libpng15"
	"${trends_buildtool_prefix}/gtk3/include/pango-1.0"
	"${trends_buildtool_prefix}/gtk3/include/pixman-1"	
	"${trends_buildtool_prefix}/gtk3/lib/glib-2.0/include"
)

LINK_DIRECTORIES(
	"${trends_srcpath_prefix}/curl-7.48.0/dist/lib"
	"${trends_srcpath_prefix}/zlib-1.2.8/dist/lib"
	"${trends_srcpath_prefix}/openssl-1.0.2g/dist/lib"
	"${trends_incpath_prefix}/klib/lib"
	"${trends_incpath_prefix}/log4c/lib"
	"${trends_buildtool_prefix}/gtk3/lib"
)
## ADD_LIBRARY
## Use all the *.cpp files we found under this folder for the project
## FILE(GLOB SRCS "*.cpp" "*.c")
## FILE(GLOB_RECURSE SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp") 
FILE(GLOB_RECURSE SRCS  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c* ${CMAKE_CURRENT_SOURCE_DIR}/src/common/*.c*)
add_executable(${PROJECT_NAME} ${SRCS})
TARGET_LINK_LIBRARIES(TRENDS gtk-3 gdk-3 gdi32 imm32 shell32 ole32 uuid pangocairo-1.0 freetype fontconfig pangowin32-1.0 gdi32 pango-1.0 m atk-1.0 cairo-gobject cairo gdk_pixbuf-2.0 gio-2.0 gobject-2.0 glib-2.0 intl libcurl.a libssh2.a  libz.a libssl.a libcrypto.a wldap32 ws2_32)
## generate SHARE_OR_STATIC lib
## add_library(pr SHARED pr.cpp)
## add_library(prs STATIC pr.cpp)
## Define the executable
## add_executable(${PROJECT_NAME} ${SRCS})
## ADD_EXECUTABLE(add_Item add_Item.cpp)
## ADD_EXECUTABLE(main main.cpp)

## TARGET_LINK_LIBRARIES(WINAPP log4c)
## FILE(GLOB TEST  ${CMAKE_CURRENT_SOURCE_DIR}/test/*.c*)
## add_executable(${TEST_NAME} ${TEST})
## TARGET_LINK_LIBRARIES(WINAPP log4c.a expat)
## add_subdirectory(include)
FILE(GLOB TESTGTKSRCS ${CMAKE_CURRENT_SOURCE_DIR}/test/testgtk.c)
add_executable(${TESTGTK_NAME} ${TESTGTKSRCS})
TARGET_LINK_LIBRARIES(TESTGTK gtk-3 gdk-3 gdi32 imm32 shell32 ole32 uuid pangocairo-1.0 freetype fontconfig pangowin32-1.0 gdi32 pango-1.0 m atk-1.0 cairo-gobject cairo gdk_pixbuf-2.0 gio-2.0 gobject-2.0 glib-2.0 intl libcurl.a libssh2.a libssl.a libcrypto.a libz.a wldap32 ws2_32)
#FIND_PATH(Headers test.h)
#IF(Headers)
#        INCLUDE_DIRECTORIES(${Headers})
#ENDIF(Headers)
#FIND_LIBRARY(Testslib libtest.a)
#FIND_LIBRARY(Testdlib libtest.so)
#IF(Testslib)
#        TARGET_LINK_LIBRARIES(main ${Testslib})
#ELSE(Testslib)
#        TARGET_LINK_LIBRARIES(main ${Testdlib})
#ENDIF(Testslib)
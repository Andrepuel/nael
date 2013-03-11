CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

PROJECT( nael )

LIST( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules )
INCLUDE( source_group_tree )

if(CMAKE_COMPILER_IS_GNUCXX OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall")
endif()

SET( NAEL_ROOT ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "nael root" )
SET( NAEL_INCLUDE_DIRS ${NAEL_ROOT}/include "nael include dirs" )

INCLUDE_DIRECTORIES( ${NAEL_INCLUDE_DIRS} )

FILE( GLOB_RECURSE sources ${NAEL_ROOT}/src/*.cpp )
FILE( GLOB_RECURSE headers ${NAEL_ROOT}/include/*.hpp )

source_group_tree("Header Files" include)
source_group_tree("Source Files" src)

ADD_EXECUTABLE( nael ${sources} ${headers} )

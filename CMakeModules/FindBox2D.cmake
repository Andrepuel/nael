# Locate Box2D
# This module defines XXX_FOUND, XXX_INCLUDE_DIRS and XXX_LIBRARIES standard variables

FIND_PATH(BOX2D_INCLUDE_DIR Box2D.h
	HINTS
	$ENV{BOX2D_DIR}
	$ENV{BOX2D_PATH}
	${ADDITIONAL_SEARCH_PATHS}
	PATH_SUFFIXES include Include
	PATHS
		~/Library/Frameworks
		/Library/Frameworks
		/usr/local
		/usr
		/sw # Fink
		/opt/local # DarwinPorts
		/opt/csw # Blastwave
		/opt
)

FIND_LIBRARY(BOX2D_LIBRARY
	NAMES box2d
	HINTS
	$ENV{BOX2D_DIR}
	$ENV{BOX2D_PATH}
	${ADDITIONAL_SEARCH_PATHS}
	PATH_SUFFIXES lib64 lib lib/release Library
	PATHS
		~/Library/Frameworks
		/Library/Frameworks
		/usr/local
		/usr
		/sw
		/opt/local
		/opt/csw
		/opt
)

FIND_LIBRARY(BOX2D_LIBRARY_DEBUG 
	NAMES box2dd box2d_d
	HINTS
	$ENV{BOX2D_DIR}
	$ENV{BOX2D_PATH}
	${ADDITIONAL_SEARCH_PATHS}
	PATH_SUFFIXES lib64 lib lib/debug Library
	PATHS
		~/Library/Frameworks
		/Library/Frameworks
		/usr/local
		/usr
		/sw
		/opt/local
		/opt/csw
		/opt
)
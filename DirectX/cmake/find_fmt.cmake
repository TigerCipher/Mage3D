
message(STATUS "Looking for FMT library")

SET( FMT_SEARCH_PATHS
	./libs/fmt
)

find_path(FMT_INCLUDE_DIRS
	NAMES fmt/core.h
	PATHS ${FMT_SEARCH_PATHS}
	PATH_SUFFIXES include
	DOC "Find the folder with fmt/core.h")

find_library(FMT_LIBRARIES
	NAMES fmtd
	PATHS ${FMT_SEARCH_PATHS}
	PATH_SUFFIXES debug/lib
	DOC "The FMT library")

if(FMT_INCLUDE_DIRS AND FMT_LIBRARIES)
	set(FMT_FOUND TRUE)
	message(STATUS "Found FMT library")
else()
	set(FMT_FOUND FALSE)
	message(STATUS "Could not find FMT library")
endif()

message(STATUS "Looking for spdlog library")

SET( SPDLOG_SEARCH_PATHS
	./libs/spdlog
)

find_path(SPDLOG_INCLUDE_DIRS
	NAMES spdlog/logger.h
	PATHS ${SPDLOG_SEARCH_PATHS}
	PATH_SUFFIXES include
	DOC "Find the folder with spdlog/logger.h")

find_library(SPDLOG_LIBRARIES
	NAMES spdlogd
	PATHS ${SPDLOG_SEARCH_PATHS}
	PATH_SUFFIXES debug/lib
	DOC "The spdlog library")

if(SPDLOG_INCLUDE_DIRS AND SPDLOG_LIBRARIES)
	set(SPDLOG_FOUND TRUE)
	message(STATUS "Found spdlog library")
else()
	set(SPDLOG_FOUND FALSE)
	message(STATUS "Could not find spdlog library")
endif()
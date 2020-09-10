
message(STATUS "Looking for BMD library")

SET( BMD_SEARCH_PATHS
	./lib/bmd
	./libs/bmd
	# Common library locations on various operating systems (pretty pointless for now since BMD isn't released yet, but might as well include these for future sake)
	"$ENV{PROGRAMFILES}/bmd"			# WINDOWS
	"$ENV{PROGRAMFILES\(X86\)}/bmd"		# WINDOWS
	~/Library/Frameworks				# MAC
	/Library/Frameworks					# MAC
	/usr/local							# LINUX/MAC/UNIX
	/usr								# LINUX/MAC/UNIX
	/opt								# LINUX/MAC/UNIX
	/sw									# Fink
	/opt/local							# DarwinPorts
	/opt/csw							# Blastwave
)

find_path(BMD_INCLUDE_DIRS
	NAMES bmd/common.h
	PATHS ${BMD_SEARCH_PATHS}
	PATH_SUFFIXES include
	DOC "Find the folder with bmd/common.h")

find_library(BMD_LIBRARIES
	NAMES bmd BMD libbmd.a
	PATHS ${BMD_SEARCH_PATHS}
	PATH_SUFFIXES lib64 lib/Release/x64 lib/Debug/x64
	DOC "The BMD library")

if(BMD_INCLUDE_DIRS AND BMD_LIBRARIES)
	set(BMD_FOUND TRUE)
	message(STATUS "Found BMD library")
else()
	set(BMD_FOUND FALSE)
	message(STATUS "Could not find BMD library")
endif()
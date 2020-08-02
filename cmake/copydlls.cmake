cmake_minimum_required(VERSION 3.16...3.18)
message(STATUS "Resolving dependencies for ${EXEPATH}")

# Wayyyy too slow
#file(GET_RUNTIME_DEPENDENCIES
#	PRE_EXCLUDE_REGEXES ${DEST}/.*
#	POST_EXCLUDE_REGEXES ${DEST}/.*
#	EXECUTABLES ${EXEPATH}
#	RESOLVED_DEPENDENCIES_VAR DEPS
#	UNRESOLVED_DEPENDENCIES_VAR UNRES_DEPS
#	DIRECTORIES ${DLL_PATH}
#	)

# message(STATUS "Resolved dependencies: ${DEPS}")
# message(STATUS "Unresolved dependencies: ${UNRES_DEPS}")

message(STATUS "Project DLL path: ${DLL_PATH}")

# Convert to regex friendly path just in case
string(REGEX REPLACE "([][+.*()^])" "\\\\\\1" regex "${DLL_PATH}")
message(STATUS "Regex friendly path: ${regex}")

message(STATUS "Copying DLLs to ${DEST}")

# CMAKE_SHARED_LIBRARY_SUFFIX -> .dll on windows

set(DEPS ${DLL_PATH}/fmtd.dll ${DLL_PATH}/glew32d.dll ${DLL_PATH}/glfw3.dll ${DLL_PATH}/mage3d.dll)


foreach(DEP IN LISTS DEPS)
	# Should dll be copied? if so then don't set the COPY_DEP variable
	string(REGEX MATCHALL "^${regex}/.[(A-z)|(a-z)|(0-9)]*\\.dll" FOUND_PATH ${DEP}) # Make this accept dll and mac/linux variants
#	if(FOUND_PATH)
#		set(COPY_DEP TRUE)
#		get_filename_component(DEP_NO_PATH ${DEP} NAME_WE)
#		get_filename_component(DEP_EXT ${DEP} EXT)
#		message(STATUS "${DEP_NO_PATH}${DEP_EXT} found at ${DLL_PATH} and set to copy!")
#	endif()
#	if(NOT COPY_DEP)
#		continue()
#	endif()

	message(STATUS "Copying ${DEP} to ${DEST}")
	file(COPY "${DEP}" DESTINATION "${DEST}")
#	set(COPY_DEP)
endforeach()
set(DEFAULT_BUILD_TYPE "Debug") #release doesnt seem to build quite right rn, so ignoring release rn
# If the .git folder exists, its probably someone working on code so lets make it debug
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
	message(AUTHOR_WARNING "Found .git folder, setting default build type to Debug")
	set(DEFAULT_BUILD_TYPE "Debug")
endif()

# CMAKE_CONFIGURATION_TYPES - Some generators, like Visual Studio can build multi configurations
# Usually it would be good to add AND NOT CMAKE_CONFIGURATION_TYPES but then my dll copy system would break
# Might be good to make the dll copy only work when in debug mode?
if(NOT CMAKE_BUILD_TYPE)
  message(AUTHOR_WARNING "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
else()
	message(STATUS "Build type already set!")
endif()
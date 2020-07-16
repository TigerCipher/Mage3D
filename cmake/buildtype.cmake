set(DEFAULT_BUILD_TYPE "Debug") #release doesnt seem to build quite right rn, so ignoring release rn
# If the .git folder exists, its probably someone working on code so lets make it debug
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
	message(Magenta "Found .git folder, setting default build type to Debug")
	set(DEFAULT_BUILD_TYPE "Debug")
endif()


# Some generators, like Visual Studio, support multiple configurations, in this case, don't set the BUILD_TYPE as it can be selected in the IDE
# Might need to force a type though to support adding compile options conditionally based on config type?
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(Magenta "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
else()
	message(Magenta "Build type either already set or generator supports multiple configurations!")
endif()
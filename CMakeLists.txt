cmake_minimum_required(VERSION 3.16...3.18)

project(Mage3D
VERSION 0.1
DESCRIPTION "An OpenGL Game engine, mostly focusing on graphics"
LANGUAGES CXX)

set(CMAKE_COLOR_MAKEFILE ON)
set(LIB_TYPE "BOTH" CACHE STRING "The type of library to build")
set(MAGE3D_SHARED "mage3d")
set(MAGE3D_STATIC "mage3ds")

#SET(BUILD_SHARED_LIBS ON)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
INCLUDE(coloredmessage)
INCLUDE(buildtype)

if(NOT CMAKE_TOOLCHAIN_FILE OR NOT _VCPKG_INSTALLED_DIR)
	message(WARNING "It is recommended you use the provided shell scripts to set the project up")
	message(FATAL_ERROR "You must supply the vcpkg tool chain file for this cmake system -DCMAKE_TOOLCHAIN_FILE=path/to/vcpkg.cmake")
endif()

message(STATUS "Beginning build...")
message(STATUS "Building ${PROJECT_NAME} for ${CMAKE_GENERATOR}")
message(STATUS "Cmake prefix: ${CMAKE_PREFIX_PATH}")
message(STATUS "Toolchain: ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "VCPKG Install dir: ${_VCPKG_INSTALLED_DIR}")
message(STATUS "VCPKG Target Triplet: ${VCPKG_TARGET_TRIPLET}")
message(STATUS "Sys path: ${CMAKE_SYSTEM_PREFIX_PATH}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")


# Not sure this is actually needed anymore
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(Mage3D_VCPKG_BIN "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug/bin")
else(CMAKE_BUILD_TYPE STREQUAL "Release")
	set(Mage3D_VCPKG_BIN "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin")
endif()

message(STATUS "VCPKG Bin location is: ${Mage3D_VCPKG_BIN}")

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  # Could do things like CMAKE_CXX_STANDARD here

  # We want std=c++xx not g++
  set(CMAKE_CXX_EXTENSIONS OFF)

  # Lets us use folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  # Enable testing for the main application
  #include(CTest)

# Will deal with doxygen and documentation much later
#  find_package(Doxygen)
#  if(Doxygen_FOUND)
#    add_subdirectory(docs)
#  else()
#    message(STATUS "Doxygen not found, cannot build docs")
#  endif()
endif()

# Was using boost to test and learn cmake, don't think I will actually use it though
#find_package(Boost REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

find_package(fmt CONFIG REQUIRED)

#include(FetchContent) Only using this in tests/CMakeLists.txt which I currently have disabled while I still learn about Check2

add_subdirectory(src)
add_subdirectory(app)

# Still learning about Check2 so for now tests are pointless
#if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING) AND BUILD_TESTING)
#  add_subdirectory(tests)
#endif()
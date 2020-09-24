cmake_minimum_required(VERSION 3.16...3.18)

project(Mage3DX
VERSION 0.1
DESCRIPTION "A DirectX 11 version of my Mage3D engine. Mostly for my own learning purposes"
LANGUAGES CXX)

#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE ON)
# I haven't decided yet if my main opengl engine will have an editor or be a pure API,
# But I plan to make this directX version an editor to some degree, so at least for now
# I am building it as an executable rather than a library
# set(LIB_TYPE "SHARED" CACHE STRING "The type of library to build")
# set(MAGE3D_SHARED "mage3d")
# set(MAGE3D_STATIC "mage3ds")

#SET(BUILD_SHARED_LIBS ON)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
INCLUDE(coloredmessage)
INCLUDE(buildtype)


message(STATUS "Beginning build...")


# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  # Could do things like CMAKE_CXX_STANDARD here

  # We want std=c++xx not g++
  set(CMAKE_CXX_EXTENSIONS OFF)
#  set(CMAKE_CXX_FLAGS "/O2")

  # Lets us use folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  # Enable testing for the main application
  #include(CTest)

# Will deal with doxygen and documentation much later
  find_package(Doxygen)
  if(Doxygen_FOUND)
      message(STATUS "Found Doxygen")
    add_subdirectory(docs)
  else()
    message(STATUS "Doxygen not found, cannot build docs")
  endif()
endif()


include(find_bmd)
include(find_spdlog)
include(find_fmt)

#include(FetchContent) Only using this in tests/CMakeLists.txt which I currently have disabled while I still learn about Check2

add_subdirectory(src)
# add_subdirectory(app)

# Still learning about Check2 so for now tests are pointless
#if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING) AND BUILD_TESTING)
#  add_subdirectory(tests)
#endif()
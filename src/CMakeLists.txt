
file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS RELATIVE "${Mage3D_SOURCE_DIR}/include/mage"
	*.h
	*.hpp
	)
file(GLOB_RECURSE SRC_LIST CONFIGURE_DEPENDS RELATIVE "${Mage3D_SOURCE_DIR}/src"
	*.cpp
	*.cc
	*.cxx
	*.c
	)

# if(CMAKE_BUILD_TYPE STREQUAL "Release")
# 	add_definitions(-O2)
# endif()


include(GenerateExportHeader)

add_library(mage3d SHARED ${SRC_LIST} ${HEADER_LIST})
#maybe call it mage3d_static for better clarification?
add_library(libmage3d STATIC ${SRC_LIST} ${HEADER_LIST})

# Generates both the DLL and the .Lib to link to
# Needs testing on non windows to see if things like .so and .a and whatnot get generated instead

set(EXPORT_FILE "${CMAKE_SOURCE_DIR}/include/mage3d_exported.h")
message(STATUS "Generating shared library export header file: ${EXPORT_FILE}")

GENERATE_EXPORT_HEADER(mage3d
	BASE_NAME mage3d
	EXPORT_MACRO_NAME mage3d_EXPORT
	EXPORT_FILE_NAME ${EXPORT_FILE}
#	STATIC_DEFINE mage3d_BUILT_AS_STATIC
	)


set(MAGE3D_LINKS ${OPENGL_LIBRARY} GLEW::GLEW)
set(MAGE3D_INCLUDES "../include")

message(STATUS "Setting include directories: ${MAGE3D_INCLUDES}")
target_include_directories(mage3d PUBLIC ${MAGE3D_INCLUDES})
target_include_directories(libmage3d PUBLIC ${MAGE3D_INCLUDES})

message(STATUS "Linking libraries ${MAGE3D_LINKS}")
target_link_libraries(mage3d ${MAGE3D_LINKS})
target_link_libraries(libmage3d ${MAGE3D_LINKS})

message(STATUS "Setting combile features")
target_compile_features(mage3d PUBLIC cxx_std_11)
target_compile_features(libmage3d PUBLIC cxx_std_11)

message(STATUS "Setting static preprocessor for static build libmage3d")
set_target_properties(libmage3d PROPERTIES COMPILE_FLAGS -DMAGE3D_STATIC_DEFINE)



source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})

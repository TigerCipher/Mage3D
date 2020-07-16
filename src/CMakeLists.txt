
file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS RELATIVE "${Mage3D_SOURCE_DIR}/src"
	${Mage3D_SOURCE_DIR}/include/*.h
	${Mage3D_SOURCE_DIR}/include/*.hpp
	)

file(GLOB_RECURSE SRC_LIST CONFIGURE_DEPENDS RELATIVE "${Mage3D_SOURCE_DIR}/src"
	${Mage3D_SOURCE_DIR}/src/*.cpp
	${Mage3D_SOURCE_DIR}/src/*.cc
	${Mage3D_SOURCE_DIR}/src/*.cxx
	${Mage3D_SOURCE_DIR}/src/*.c
	)
# if(CMAKE_BUILD_TYPE STREQUAL "Release")
# 	add_definitions(-O2)
# endif()



include(GenerateExportHeader)
include(filter)

set(MAGE3D_LINKS ${OPENGL_LIBRARY} GLEW::GLEW)
set(MAGE3D_INCLUDES "../include")

if(LIB_TYPE STREQUAL "SHARED")
	message(AUTHOR_WARNING "Src: ${SRC_LIST} and Headers: ${HEADER_LIST}")
	add_library(mage3d SHARED ${SRC_LIST} ${HEADER_LIST})
	define_property(TARGET PROPERTY HEADERS BRIEF_DOCS "Header files for the target" FULL_DOCS "Header files for the target to be used in source grouping")
	set_property(TARGET mage3d PROPERTY HEADERS ${HEADER_LIST})
	message(STATUS "Setting include directories: ${MAGE3D_INCLUDES}")
	target_include_directories(mage3d PUBLIC ${MAGE3D_INCLUDES})
	message(STATUS "Linking libraries ${MAGE3D_LINKS}")
	target_link_libraries(mage3d ${MAGE3D_LINKS})
	message(STATUS "Setting compile features")
	target_compile_features(mage3d PUBLIC cxx_std_11)
else()
	#maybe call it mage3d_static for better clarification?
	add_library(libmage3d STATIC ${SRC_LIST} ${HEADER_LIST})
	message(STATUS "Setting include directories: ${MAGE3D_INCLUDES}")
	target_include_directories(libmage3d PUBLIC ${MAGE3D_INCLUDES})
	message(STATUS "Linking libraries ${MAGE3D_LINKS}")
	target_link_libraries(libmage3d ${MAGE3D_LINKS})
	message(STATUS "Setting compile features")
	target_compile_features(libmage3d PUBLIC cxx_std_11)
	message(STATUS "Setting static preprocessor for static build libmage3d")
	set_target_properties(libmage3d PROPERTIES COMPILE_FLAGS -DMAGE3D_STATIC_DEFINE)
endif()


# Generates both the DLL and the .Lib to link to
# Needs testing on non windows to see if things like .so and .a and whatnot get generated instead

set(EXPORT_FILE "${CMAKE_SOURCE_DIR}/include/mage3d_exported.h")
message(STATUS "Generating library export header file: ${EXPORT_FILE}")

GENERATE_EXPORT_HEADER(mage3d
	BASE_NAME mage3d
	EXPORT_MACRO_NAME mage3d_EXPORT
	EXPORT_FILE_NAME ${EXPORT_FILE}
#	STATIC_DEFINE mage3d_BUILT_AS_STATIC
	)

target_source_group(ROOT_DIR "${PROJECT_SOURCE_DIR}/include" TARGET mage3d PREFIX "Header Files")
#source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})
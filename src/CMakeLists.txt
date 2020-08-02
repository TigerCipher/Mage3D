
file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS
	${Mage3D_SOURCE_DIR}/include/*.h
	${Mage3D_SOURCE_DIR}/include/*.hpp
	)

file(GLOB_RECURSE SRC_LIST CONFIGURE_DEPENDS
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

set(MAGE3D_LINKS PUBLIC ${BMD_LIBRARIES} fmt::fmt ${OPENGL_LIBRARY} GLEW::GLEW glfw SOIL glm)
set(MAGE3D_INCLUDES "../include" ${BMD_INCLUDE_DIRS})


message(STATUS "Lib type is ${LIB_TYPE}")

if(LIB_TYPE STREQUAL "BOTH")

	# Shared lib
	message(Blue "Adding shared library ${MAGE3D_SHARED}")

	add_library(${MAGE3D_SHARED} SHARED ${SRC_LIST} ${HEADER_LIST})

	define_property(TARGET PROPERTY HEADERS BRIEF_DOCS "Header files for the target" FULL_DOCS "Header files for the target to be used in source grouping")
	set_property(TARGET ${MAGE3D_SHARED} PROPERTY HEADERS ${HEADER_LIST})

	define_property(TARGET PROPERTY SRCS BRIEF_DOCS "Header files for the target" FULL_DOCS "Header files for the target to be used in source grouping")
	set_property(TARGET ${MAGE3D_SHARED} PROPERTY SRCS ${SRC_LIST})

	message(STATUS "Setting include directories for ${MAGE3D_SHARED}, dirs: ${MAGE3D_INCLUDES}")
	target_include_directories(${MAGE3D_SHARED} PUBLIC ${MAGE3D_INCLUDES})
	message(STATUS "Linking libraries to ${MAGE3D_SHARED}, libs: ${MAGE3D_LINKS}")
	target_link_libraries(${MAGE3D_SHARED} ${MAGE3D_LINKS})
	message(STATUS "Setting compile features for ${MAGE3D_SHARED}")
	target_compile_features(${MAGE3D_SHARED} PUBLIC cxx_std_11)


	# Static lib
	message(Blue "Adding static library ${MAGE3D_STATIC}")
	add_library(${MAGE3D_STATIC} STATIC ${SRC_LIST} ${HEADER_LIST})

	define_property(TARGET PROPERTY HEADERS BRIEF_DOCS "Header files for the target" FULL_DOCS "Header files for the target to be used in source grouping")
	set_property(TARGET ${MAGE3D_STATIC} PROPERTY HEADERS ${HEADER_LIST})

	define_property(TARGET PROPERTY SRCS BRIEF_DOCS "Header files for the target" FULL_DOCS "Header files for the target to be used in source grouping")
	set_property(TARGET ${MAGE3D_STATIC} PROPERTY SRCS ${SRC_LIST})



	message(STATUS "Setting include directories for ${MAGE3D_STATIC}, dirs: ${MAGE3D_INCLUDES}")
	target_include_directories(${MAGE3D_STATIC} PUBLIC ${MAGE3D_INCLUDES})
	message(STATUS "Linking libraries to ${MAGE3D_STATIC}, libs: ${MAGE3D_LINKS}")
	target_link_libraries(${MAGE3D_STATIC} ${MAGE3D_LINKS})
	message(STATUS "Setting compile features for ${MAGE3D_STATIC}")
	target_compile_features(${MAGE3D_STATIC} PUBLIC cxx_std_11)
	message(STATUS "Setting static preprocessor for static build ${MAGE3D_STATIC}")
	set_target_properties(${MAGE3D_STATIC} PROPERTIES COMPILE_FLAGS -DMAGE3D_STATIC_DEFINE)
elseif(LIB_TYPE STREQUAL "STATIC")
	set(MAGE3D_TARGET ${MAGE3D_STATIC})
	message(Blue "Adding static library ${MAGE3D_STATIC}")
	add_library(${MAGE3D_STATIC} STATIC ${SRC_LIST} ${HEADER_LIST})
	message(STATUS "Setting include directories for ${MAGE3D_STATIC}, dirs: ${MAGE3D_INCLUDES}")

	define_property(TARGET PROPERTY HEADERS BRIEF_DOCS "Header files for the target" FULL_DOCS "Header files for the target to be used in source grouping")
	set_property(TARGET ${MAGE3D_STATIC} PROPERTY HEADERS ${HEADER_LIST})

	define_property(TARGET PROPERTY SRCS BRIEF_DOCS "Header files for the target" FULL_DOCS "Header files for the target to be used in source grouping")
	set_property(TARGET ${MAGE3D_STATIC} PROPERTY SRCS ${SRC_LIST})


	target_include_directories(${MAGE3D_STATIC} PUBLIC ${MAGE3D_INCLUDES})
	message(STATUS "Linking libraries to ${MAGE3D_STATIC}, libs: ${MAGE3D_LINKS}")
	target_link_libraries(${MAGE3D_STATIC} ${MAGE3D_LINKS})
	message(STATUS "Setting compile features for ${MAGE3D_STATIC}")
	target_compile_features(${MAGE3D_STATIC} PUBLIC cxx_std_11)
	message(STATUS "Setting static preprocessor for static build ${MAGE3D_STATIC}")
	set_target_properties(${MAGE3D_STATIC} PROPERTIES COMPILE_DEFINITIONS "MAGE3D_STATIC_DEFINE")
else()
	if(LIB_TYPE STREQUAL "SHARED")
		message(Blue "Adding shared library ${MAGE3D_SHARED}")
	elseif(LIB_TYPE)
		message(WARNING "LIB_TYPE must be either SHARED, STATIC, or BOTH. ${LIB_TYPE} is invalid. Defaulting to SHARED")
	else()
		message(Blue "LIB_TYPE not specified, defaulting to SHARED")
		message(Blue "To specify the type of library to build, pass in -DLIB_TYPE=[SHARED | STATIC | BOTH]")
		set(LIB_TYPE "SHARED")
	endif()
	set(MAGE3D_TARGET ${MAGE3D_SHARED})
	add_library(${MAGE3D_SHARED} SHARED ${SRC_LIST} ${HEADER_LIST})

	define_property(TARGET PROPERTY HEADERS BRIEF_DOCS "Header files for the target" FULL_DOCS "Header files for the target to be used in source grouping")
	set_property(TARGET ${MAGE3D_SHARED} PROPERTY HEADERS ${HEADER_LIST})

	define_property(TARGET PROPERTY SRCS BRIEF_DOCS "Header files for the target" FULL_DOCS "Header files for the target to be used in source grouping")
	set_property(TARGET ${MAGE3D_SHARED} PROPERTY SRCS ${SRC_LIST})

	message(STATUS "Setting include directories for ${MAGE3D_SHARED}, dirs: ${MAGE3D_INCLUDES}")
	target_include_directories(${MAGE3D_SHARED} PUBLIC ${MAGE3D_INCLUDES})
	message(STATUS "Linking libraries to ${MAGE3D_SHARED}, libs: ${MAGE3D_LINKS}")
	target_link_libraries(${MAGE3D_SHARED} ${MAGE3D_LINKS})
	message(STATUS "Setting compile features for ${MAGE3D_SHARED}")
	target_compile_features(${MAGE3D_SHARED} PUBLIC cxx_std_11)
endif()


# Generates both the DLL and the .Lib to link to
# Needs testing on non windows to see if things like .so and .a and whatnot get generated instead

set(EXPORT_FILE "${CMAKE_SOURCE_DIR}/include/mage3d_exported.h")
message(STATUS "Generating library export header file: ${EXPORT_FILE}")

GENERATE_EXPORT_HEADER(${MAGE3D_TARGET}
	BASE_NAME mage3d
	EXPORT_MACRO_NAME mage3d_EXPORT
	EXPORT_FILE_NAME ${EXPORT_FILE}
	)


if(LIB_TYPE STREQUAL "BOTH")
	message(Blue "Creating folders for projects ${MAGE3D_SHARED} and ${MAGE3D_STATIC}")
	target_source_group(
					ROOT_DIR "${CMAKE_SOURCE_DIR}/src" "${CMAKE_SOURCE_DIR}/include"
					TARGET ${MAGE3D_SHARED} ${MAGE3D_STATIC}
					PREFIX "Source Files" "Header Files")
elseif(LIB_TYPE STREQUAL "SHARED")
	message(Blue "Creating folders for project ${MAGE3D_SHARED}")
	target_source_group(
					ROOT_DIR "${CMAKE_SOURCE_DIR}/src" "${CMAKE_SOURCE_DIR}/include"
					TARGET ${MAGE3D_SHARED}
					PREFIX "Source Files" "Header Files")
else()
	message(Blue "Creating folders for project ${MAGE3D_STATIC}")
	target_source_group(
					ROOT_DIR "${CMAKE_SOURCE_DIR}/src" "${CMAKE_SOURCE_DIR}/include"
					TARGET ${MAGE3D_STATIC}
					PREFIX "Source Files" "Header Files")
endif()
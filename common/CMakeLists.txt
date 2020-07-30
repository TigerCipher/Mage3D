
file(GLOB_RECURSE COMMON_HEADERS CONFIGURE_DEPENDS
	${Mage3D_SOURCE_DIR}/common/include/*.h
	${Mage3D_SOURCE_DIR}/common/include/*.hpp
	)

file(GLOB_RECURSE COMMON_SOURCES CONFIGURE_DEPENDS
	${Mage3D_SOURCE_DIR}/common/src/*.cpp
	${Mage3D_SOURCE_DIR}/common/src/*.cc
	${Mage3D_SOURCE_DIR}/common/src/*.cxx
	${Mage3D_SOURCE_DIR}/common/src/*.c
	)

set(COMMON_INCLUDES "include")

include(filter)

add_library(blutilities STATIC ${COMMON_SOURCES} ${COMMON_HEADERS})

define_property(TARGET PROPERTY HEADERS BRIEF_DOCS "Header files for the target" FULL_DOCS "Header files for the target to be used in source grouping")
set_property(TARGET blutilities PROPERTY HEADERS ${COMMON_HEADERS})
define_property(TARGET PROPERTY SRCS BRIEF_DOCS "Header files for the target" FULL_DOCS "Header files for the target to be used in source grouping")
set_property(TARGET blutilities PROPERTY SRCS ${COMMON_SOURCES})

target_include_directories(blutilities PUBLIC ${COMMON_INCLUDES})
target_compile_features(blutilities PUBLIC cxx_std_11)

target_source_group(
	ROOT_DIR "${CMAKE_SOURCE_DIR}/common/src" "${CMAKE_SOURCE_DIR}/common/include"
	TARGET blutilities
	PREFIX "Source Files" "Header Files"
	)

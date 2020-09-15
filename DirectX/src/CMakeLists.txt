
file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS
	${Mage3DX_SOURCE_DIR}/include/*.h
	${Mage3DX_SOURCE_DIR}/include/*.hpp
	)

file(GLOB_RECURSE SRC_LIST CONFIGURE_DEPENDS
	${Mage3DX_SOURCE_DIR}/src/*.cpp
	${Mage3DX_SOURCE_DIR}/src/*.cc
	${Mage3DX_SOURCE_DIR}/src/*.cxx
	${Mage3DX_SOURCE_DIR}/src/*.c
	${Mage3DX_SOURCE_DIR}/src/*.rc
	)



include(filter)

#TODO StalkWalker is only compatible with MSVC. If I end up liking spdlog, maybe just get rid of stack walker from my bmd logger
set(MAGE3D_LINKS PUBLIC ${BMD_LIBRARIES})
set(MAGE3D_INCLUDES "../include" ${BMD_INCLUDE_DIRS})

message(Blue "Creating Mage3DX executable")
add_executable(mage3dx WIN32 ${SRC_LIST} ${HEADER_LIST})
define_property(TARGET PROPERTY HEADERS BRIEF_DOCS "Header files for the target" FULL_DOCS "Header files for the target to be used in source grouping")
set_property(TARGET mage3dx PROPERTY HEADERS ${HEADER_LIST})

define_property(TARGET PROPERTY SRCS BRIEF_DOCS "Header files for the target" FULL_DOCS "Header files for the target to be used in source grouping")
set_property(TARGET mage3dx PROPERTY SRCS ${SRC_LIST})

message(STATUS "Setting include directories for mage3dx, dirs: ${MAGE3D_INCLUDES}")
target_include_directories(mage3dx PUBLIC ${MAGE3D_INCLUDES})
message(STATUS "Linking libraries to mage3dx, libs: ${MAGE3D_LINKS}")
target_link_libraries(mage3dx ${MAGE3D_LINKS})
message(STATUS "Setting compile features for mage3dx")
target_compile_features(mage3dx PUBLIC cxx_std_17)

# NEEDS TO BE DIFFERENT FOR RELEASE! (but for release I'll probably just build on Visual Studio instead of cmake)
target_compile_options(mage3dx PUBLIC /MP /Ot /fp:fast -MTd)

target_precompile_headers(mage3dx PRIVATE "../include/mage_pch.h")




message(Blue "Creating folders for project mage3dx")
target_source_group(
				ROOT_DIR "${CMAKE_SOURCE_DIR}/src" "${CMAKE_SOURCE_DIR}/include"
				TARGET mage3dx
				PREFIX "Source Files" "Header Files")
# Get the source files for the executable (which is only used for testing the engine and not for production)
file(GLOB_RECURSE APP_SRC_LIST CONFIGURE_DEPENDS RELATIVE "${Mage3D_SOURCE_DIR}/app"
	*.cpp
	*.cc
	*.cxx
	*.c
	)

add_executable(app ${APP_SRC_LIST})

message(STATUS "Setting compile features for the app")
target_compile_features(app PRIVATE cxx_std_17)

# mage3d = shared, libmage3d = static
set(MAGE3D_WHICH_LIB mage3d)

message(STATUS "Linking app libraries")
target_link_libraries(app PUBLIC ${MAGE3D_WHICH_LIB})
target_link_libraries(app PRIVATE fmt::fmt)


if(MAGE3D_WHICH_LIB STREQUAL "mage3d")
	message(STATUS "Building app with the shared build ${MAGE3D_WHICH_LIB}")
	add_custom_command(TARGET app POST_BUILD
		COMMAND "${CMAKE_COMMAND}" 
				"-DEXEPATH=$<TARGET_FILE:app>"
				"-DDEST=${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>"
				"-DDLL_PATH=${CMAKE_BINARY_DIR}/src/$<CONFIG>"
				-P ${CMAKE_MODULE_PATH}/copydlls.cmake
		)
else()
	message(STATUS "Building app with the static build ${MAGE3D_WHICH_LIB}")
endif()
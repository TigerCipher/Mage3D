# Get the source files for the executable (which is only used for testing the engine and not for production)
file(GLOB_RECURSE APP_SRC_LIST CONFIGURE_DEPENDS
	"${Mage3D_SOURCE_DIR}/app/*.cpp"
	"${Mage3D_SOURCE_DIR}/app/*.cc"
	"${Mage3D_SOURCE_DIR}/app/*.cxx"
	"${Mage3D_SOURCE_DIR}/app/*.c"
	)

file(GLOB_RECURSE APP_HEADER_LIST CONFIGURE_DEPENDS
	"${Mage3D_SOURCE_DIR}/app/*.h"
	"${Mage3D_SOURCE_DIR}/app/*.hpp"
	)

add_executable(app ${APP_SRC_LIST} ${APP_HEADER_LIST})

message(STATUS "Setting compile features for the app")
target_compile_features(app PRIVATE cxx_std_17)

# mage3d = shared, mage3ds = static

if(LIB_TYPE STREQUAL "SHARED" OR LIB_TYPE STREQUAL "BOTH")
	message(Magenta "LIB_TYPE is set to ${LIB_TYPE} so the test app will be linked to the shared library ${MAGE3D_SHARED}")
	message(Yellow "Note: As the test app is not part of the actual release project, this doesn't really matter")
	set(MAGE3D_WHICH_LIB ${MAGE3D_SHARED})
else()
	message(Magenta "LIB_TYPE is set to ${LIB_TYPE} so the test app will be linked to the static library ${MAGE3D_STATIC}")
	message(Yellow "Note: As the test app is not part of the actual release project, this doesn't really matter")
	set(MAGE3D_WHICH_LIB ${MAGE3D_STATIC})
endif()

message(STATUS "Linking app libraries")
# Link whichever mage3d library
# I personally prefer shared libraries so the ending game executable is a smaller size
# Additionally this can allow mage3d to be installed somewhere on the environment path
# And then games can detect if the client needs to download and install mage3d or not
# If the client already has the engine files, why bother downloading them again?
target_link_libraries(app PRIVATE ${MAGE3D_WHICH_LIB})

# Libraries used by the test app but not the engine (currently this is purely for example/testing purposes.
# Any libraries I feel needed for the engine will be added to the engine)
# This is where you might add your own libraries if you want to go ahead and use the engine to make something
# but not actually modify the engine itself
# Note that if you do want to contribute to the engine, if you add additional libraries to the engine (src/CMakeLists.txt)
# There would be a less likely chance of me accepting the pull request if it is a library I am unfamiliar with
# In those cases, it's best to add here and add code to the app project to show case its usage and describe it's benefits
#target_link_libraries(app PRIVATE fmt::fmt)

file(GLOB ASSETS CONFIGURE_DEPENDS
	"${Mage3D_SOURCE_DIR}/assets/*.vert"
	"${Mage3D_SOURCE_DIR}/assets/*.frag"
	)

# Copy_if_different doesnt seem to work, seems to always copy no matter what
foreach(SHADER_ASSET IN LISTS ASSETS)
	if(CMAKE_CONFIGURATION_TYPES)
		add_custom_command(TARGET app PRE_BUILD
				COMMAND "${CMAKE_COMMAND}" -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/assets
				COMMAND "${CMAKE_COMMAND}" -E copy_if_different ${SHADER_ASSET} ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/assets
				COMMAND "${CMAKE_COMMAND}" -E copy_if_different ${SHADER_ASSET} ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/assets
				COMMENT "Copying shader: ${SHADER_ASSET}")
	else()
		add_custom_command(TARGET app PRE_BUILD
				COMMAND "${CMAKE_COMMAND}" -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/assets
				COMMAND "${CMAKE_COMMAND}" -E copy_if_different ${SHADER_ASSET} ${CMAKE_CURRENT_BINARY_DIR}/assets
				COMMENT "Copying shader: ${SHADER_ASSET}")
	endif()
endforeach()

if(LIB_TYPE STREQUAL "SHARED" OR LIB_TYPE STREQUAL "BOTH")
	message(STATUS "Building app with the shared build ${MAGE3D_WHICH_LIB}")
	if(CMAKE_CONFIGURATION_TYPES)
		add_custom_command(TARGET app PRE_BUILD
			COMMAND "${CMAKE_COMMAND}"
					"-DEXEPATH=$<TARGET_FILE:app>"
					"-DDEST=${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>"
					"-DDLL_PATH=${CMAKE_BINARY_DIR}/src/$<CONFIG>"
					-P ${CMAKE_MODULE_PATH}/copydlls.cmake
			)
	else()
		add_custom_command(TARGET app PRE_BUILD
				COMMAND "${CMAKE_COMMAND}"
				"-DEXEPATH=$<TARGET_FILE:app>"
				"-DDEST=${CMAKE_CURRENT_BINARY_DIR}"
				"-DDLL_PATH=${CMAKE_BINARY_DIR}/src"
				-P ${CMAKE_MODULE_PATH}/copydlls.cmake
				)
	endif()
else()
	message(STATUS "Building app with the static build ${MAGE3D_WHICH_LIB}")
endif()

include(filter)

source_group (
TREE    "${Mage3D_SOURCE_DIR}/app"
FILES   ${APP_HEADER_LIST}
PREFIX  "Header Files"
)

file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS
        ${Mage3DX_SOURCE_DIR}/include/*.h
        ${Mage3DX_SOURCE_DIR}/include/*.hpp
        )

list(REMOVE_ITEM HEADER_LIST "${Mage3DX_SOURCE_DIR}/include/pch.h")

file(GLOB_RECURSE SRC_LIST CONFIGURE_DEPENDS
        ${Mage3DX_SOURCE_DIR}/src/*.cpp
        ${Mage3DX_SOURCE_DIR}/src/*.cc
        ${Mage3DX_SOURCE_DIR}/src/*.cxx
        ${Mage3DX_SOURCE_DIR}/src/*.c
        ${Mage3DX_SOURCE_DIR}/src/*.rc
        )

file(GLOB_RECURSE VERTEX_SHADER_LIST CONFIGURE_DEPENDS
        ${Mage3DX_SOURCE_DIR}/assets/shaders/*VS.hlsl
        )
file(GLOB_RECURSE PIXEL_SHADER_LIST CONFIGURE_DEPENDS
        ${Mage3DX_SOURCE_DIR}/assets/shaders/*PS.hlsl
        )

include(filter)

set(MAGE3D_LINKS PUBLIC ${BMD_LIBRARIES} ${SPDLOG_LIBRARIES} ${FMT_LIBRARIES} d3d11 D3DCompiler gdiplus dxguid)
set(MAGE3D_INCLUDES "../include" ${BMD_INCLUDE_DIRS} ${SPDLOG_INCLUDE_DIRS} ${FMT_INCLUDE_DIRS})

message(Blue "Creating Mage3DX executable")
add_executable(mage3dx WIN32 ${SRC_LIST} ${HEADER_LIST} "${Mage3DX_SOURCE_DIR}/assets/application.manifest")
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
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
#if(MSVC)
#    add_compile_options(
#        $<$<CONFIG:>:/MT> #---------|
#        $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
#        $<$<CONFIG:Release>:/MT> #--|
#    )
#endif()

#set(CompilerFlags
#        CMAKE_CXX_FLAGS
#        CMAKE_CXX_FLAGS_DEBUG
#        CMAKE_CXX_FLAGS_RELEASE
#        CMAKE_CXX_FLAGS_MINSIZEREL
#        CMAKE_CXX_FLAGS_RELWITHDEBINFO
#        CMAKE_C_FLAGS
#        CMAKE_C_FLAGS_DEBUG
#        CMAKE_C_FLAGS_RELEASE
#        CMAKE_C_FLAGS_MINSIZEREL
#        CMAKE_C_FLAGS_RELWITHDEBINFO
#        )
#foreach(CompilerFlag ${CompilerFlags})
#  string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
#  set(${CompilerFlag} "${${CompilerFlag}}" CACHE STRING "msvc compiler flags" FORCE)
#  message(Yellow "MSVC flags: ${CompilerFlag}:${${CompilerFlag}}")
#endforeach()



target_compile_options(mage3dx PUBLIC /MP /Ot /fp:fast "/FI${Mage3DX_SOURCE_DIR}/include/pch.h")
#ucm_print_flags()
#    ucm_set_runtime(STATIC)
#    ucm_print_flags()

target_precompile_headers(mage3dx PRIVATE "../include/pch.h")

#foreach(hdrFile ${HEADER_LIST})
#    if(NOT ${hdrFile} STREQUAL "${Mage3DX_SOURCE_DIR}/include/pch.h")
#        set_source_files_properties(${hdrFile} PROPERTIES COMPILE_FLAGS "-include ../include/pch.h")
#    endif()
#endforeach()


include(shaders)

foreach (vs ${VERTEX_SHADER_LIST})
    compile_vs(vs_out ${vs})
    list(APPEND MAGE_VS_OUT ${vs_out})
endforeach ()
foreach (ps ${PIXEL_SHADER_LIST})
    compile_ps(ps_out ${ps})
    list(APPEND MAGE_PS_OUT ${ps_out})
endforeach ()
add_custom_target(mage_shaders DEPENDS ${MAGE_VS_OUT} ${MAGE_PS_OUT})

message(Blue "Creating folders for project mage3dx")
target_source_group(
        ROOT_DIR "${CMAKE_SOURCE_DIR}/src" "${CMAKE_SOURCE_DIR}/include"
        TARGET mage3dx
        PREFIX "Source Files" "Header Files")



get_target_property(MAIN_CFLAGS mage3dx COMPILE_OPTIONS)
# also see: COMPILE_DEFINITIONS INCLUDE_DIRECTORIES
message(STATUS "-- Target compiler flags are: ${MAIN_CFLAGS}")
message(STATUS "CXX Flags are: ${CMAKE_CXX_FLAGS_DEBUG}")

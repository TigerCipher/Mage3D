function(compile_shader var_binary_path source_path shader_type entry_point)
    get_filename_component(source_filename ${source_path} NAME_WE)
    get_filename_component(source_path "${source_path}" ABSOLUTE)
    set(binary_path "${CMAKE_CURRENT_BINARY_DIR}/${source_filename}.cso")
    add_custom_command(OUTPUT ${binary_path}
            COMMENT "Generating ${binary_path}"
            COMMAND "C:\\Program Files (x86)\\Windows Kits\\10\\bin\\10.0.19041.0\\x64\\fxc.exe" /T \"${shader_type}\" /E\"${entry_point}\" /Od /Zi /Fo \"${binary_path}\" \"${source_path}\"
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            DEPENDS "${source_path}"
            )
    set(${var_binary_path} "${binary_path}" PARENT_SCOPE)
endfunction()

macro(compile_vs var_binary_path source_path)
    compile_shader(${var_binary_path} ${source_path} "vs_5_0" "main")
endmacro()

macro(compile_ps var_binary_path source_path)
    compile_shader(${var_binary_path} ${source_path} "ps_5_0" "main")
endmacro()
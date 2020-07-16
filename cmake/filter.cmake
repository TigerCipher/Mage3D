function (target_source_group)
  set (_options
    # TODO
    )
  set (_multi_value_args
    # Required
    TARGET
    ROOT_DIR
    PREFIX
    )
  set (_one_value_args
    # TODO
    )

  cmake_parse_arguments (i
    "${_options}" "${_one_value_args}" "${_multi_value_args}" ${ARGN})
    

  # Check inputs

  foreach (_target IN LISTS i_TARGET)
    # if (option)
      # TODO: Implement this
    # else ()
      get_property(_target_sources TARGET ${_target} PROPERTY SRCS)
      get_property(_target_headers TARGET ${_target} PROPERTY HEADERS)
    # endif ()

    # Remove sources to be installed
    set (_source_to_install_regex
      "(\\$<INSTALL_INTERFACE:([^>;<$]+)>)")

    string (REGEX REPLACE
      "${_source_to_install_regex}"
      ""
      _sources_to_build
      "${_target_sources}")

    string (REGEX REPLACE
      "${_source_to_install_regex}"
      ""
      _headers_to_build
      "${_target_headers}")

    # Remove remaining ";"s. It seems safer to do it this way rather than include
    # them in _source_to_install_regex
    string (REGEX REPLACE
      "[;]+"
      ";"
      _sources_to_build
      "${_sources_to_build}")

    string (REGEX REPLACE
      "[;]+"
      ";"
      _headers_to_build
      "${_headers_to_build}")

    # Extract sources to be built
    set (_source_to_build_regex
      "\\$<BUILD_INTERFACE:([^>;<$]+)>")

    string (REGEX REPLACE
      "${_source_to_build_regex}"
      "\\1"
      _sources_to_build
      "${_sources_to_build}")
    string (REGEX REPLACE
      "${_source_to_build_regex}"
      "\\1"
      _headers_to_build
      "${_headers_to_build}")

    foreach (_root _prefix IN ZIP_LISTS i_ROOT_DIR i_PREFIX)
      set (_sources_under_root_regex
        "${_root}/[^>;<$]+")

      string (REGEX MATCHALL
        "${_sources_under_root_regex}"
        _sources_under_root
        "${_sources_to_build}")

      string (REGEX MATCHALL
        "${_sources_under_root_regex}"
        _headers_under_root
        "${_headers_to_build}")

      set(_src_group ${_sources_under_root} ${_headers_under_root})
      source_group (
        TREE    "${_root}"
        FILES   ${_src_group}
        PREFIX  "${_prefix}"
        )
    endforeach ()
  endforeach ()
endfunction (target_source_group)
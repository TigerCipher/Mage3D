function (target_source_group)
  set (_options
    GROUP_INTERFACE_SOURCES
    )
  set (_multi_value_args
    # Required
    TARGET
    ROOT_DIR
    )
  set (_one_value_args
    PREFIX
    )

  cmake_parse_arguments (i
    "${_options}" "${_one_value_args}" "${_multi_value_args}" ${ARGN})

  # Check inputs

  foreach (_target IN LISTS i_TARGET)
    if (i_GROUP_INTERFACE_SOURCES)
      get_target_property (_target_sources ${_target} INTERFACE_SOURCES)
    else ()
      get_target_property (_target_sources ${_target} SOURCES)
      # get_target_property (_target_headers ${_target} HEADERS)
      get_property(_target_headers TARGET ${_target} PROPERTY HEADERS)
    endif ()

    # Remove sources to be installed
    message(WARNING "Sources: ${_target_sources}")
    message(WARNING "HEaders: ${_target_headers}")
    set (_source_to_install_regex
      "(\\$<INSTALL_INTERFACE:([^>;<$]+)>)")

    string (REGEX REPLACE
      "${_source_to_install_regex}"
      ""
      _sources_to_build
      "${_target_sources}")

    # Remove remaining ";"s. It seems safer to do it this way rather than include
    # them in _source_to_install_regex
    string (REGEX REPLACE
      "[;]+"
      ";"
      _sources_to_build
      "${_sources_to_build}")

    # Extract sources to be built
    set (_source_to_build_regex
      "\\$<BUILD_INTERFACE:([^>;<$]+)>")

    string (REGEX REPLACE
      "${_source_to_build_regex}"
      "\\1"
      _sources_to_build
      "${_sources_to_build}")

    foreach (_root IN LISTS i_ROOT_DIR)
      set (_sources_under_root_regex
        "${_root}/[^>;<$]+")

      string (REGEX MATCHALL
        "${_sources_under_root_regex}"
        _sources_under_root
        "${_sources_to_build}")
      message(STATUS "Source group as: ${_root}  ${i_PREFIX}   ${_sources_under_root}")
      source_group (
        TREE    "${_root}"
        FILES   ${_target_headers}
        PREFIX  "${i_PREFIX}"
        )
    endforeach ()
  endforeach ()
endfunction (target_source_group)
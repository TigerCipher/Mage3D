set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_BUILTIN_STL_SUPPORT YES)

doxygen_add_docs(docs
    "${CMAKE_CURRENT_SOURCE_DIR}/mainpage.md"
    "${PROJECT_SOURCE_DIR}/common/include" #temp. Will remove once "blutilities" is more complete and I make it it's own separate project
    "${PROJECT_SOURCE_DIR}/include"
)

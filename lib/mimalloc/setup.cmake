set(MI_USE_CXX ON CACHE BOOL "")
set(MI_BUILD_STATIC OFF CACHE BOOL "")
set(MI_BUILD_OBJECT OFF CACHE BOOL "")
set(MI_BUILD_TESTS OFF CACHE BOOL "")
add_subdirectory(${path}/mimalloc)
list(APPEND metatron-deps mimalloc)

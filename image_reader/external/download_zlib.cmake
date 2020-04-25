include(ExternalProject)

set(zlib128_dir  ${CMAKE_BINARY_DIR}/zlib128)
set(zlib128_bin_dir  ${CMAKE_BINARY_DIR}/bin)
set(zlib128_src_dir  ${CMAKE_BINARY_DIR}/src)
ExternalProject_Add(
    zlib128_downloader
    GIT_REPOSITORY git@github.com:madler/zlib.git
    GIT_TAG        v1.2.8
    PREFIX         ${zlib128_src_dir}
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${zlib128_bin_dir}
)

add_library(zlib128 STATIC IMPORTED)
set_target_properties(zlib128 PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES   ${zlib128_bin_dir}/include
    IMPORTED_LOCATION_DEBUG         ${zlib128_bin_dir}/lib/zlibstaticd.lib
    IMPORTED_LOCATION_RELEASE       ${zlib128_bin_dir}/lib/zlibstatic.lib
)

add_dependencies(zlib128 zlib128_downloader)
cmake_minimum_required(VERSION 3.12)
project(image_reader)

option(INSTALL_PACKAGE "install lib to prefix if the project is referenced by superbuild" OFF)

##
#    external project
#
include(${CMAKE_CURRENT_LIST_DIR}/external/download_zlib.cmake)

##
#    config library res
#
set(IMAGE_READER_HEADER_FILES
    include/image.h
)

set(IMAGE_READER_SOURCE_FILES
    source/image/image.hpp
    source/image/image.cpp
    source/image/bmp.cpp
    source/image/png.cpp
    source/image/tga.cpp
    source/image/dds.cpp
)

source_group("include" FILES 
    ${IMAGE_READER_HEADER_FILES}
)
source_group("source" FILES
    ${IMAGE_READER_SOURCE_FILES}
)

add_library(image_reader STATIC 
    ${IMAGE_READER_HEADER_FILES}
    ${IMAGE_READER_SOURCE_FILES}
)

target_include_directories(image_reader PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(image_reader cxx_library zlib128)

##
#    install rules
#
if(INSTALL_PACKAGE)
    install(DIRECTORY include/ DESTINATION include)
    install(TARGETS image_reader DESTINATION lib)
    install(DIRECTORY ${zlib128_bin_dir}/lib/ DESTINATION lib)
endif()
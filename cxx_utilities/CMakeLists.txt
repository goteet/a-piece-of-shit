cmake_minimum_required(VERSION 3.11)

project(cxx_utilities_library)

option(INSTALL_PACKAGE  "install headers to prefix if the project is referenced by superbuild" OFF)


##
#   cxx header-only library
#
add_library(cxx_library INTERFACE)
add_library(Upstream::cxx_library ALIAS cxx_library)

target_include_directories(cxx_library INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_options(cxx_library INTERFACE /wd4996)

set(cxx_utilities_header_files
    include/cxx_common.h
    include/cxx_view.h
    include/cxx_scope.h
    include/cxx_file.h
    include/cxx_math.h
)

set(cxx_utilities_math_header_files
    include/cxx_math/cxx_mathcommon.h
    include/cxx_math/cxx_mathutility.h
    include/cxx_math/cxx_angle.h
    include/cxx_math/cxx_vector.h
    include/cxx_math/cxx_matrix.h
    include/cxx_math/cxx_rotation.h
    include/cxx_math/cxx_color.h
    include/cxx_math/cxx_point.h
    include/cxx_math/cxx_ray.h
    include/cxx_math/cxx_aabb.h
)

set(cxx_utilities_math_source_files
    #angle
    include/cxx_math/cxx_base_angle.h
    include/cxx_math/cxx_degree.h
    include/cxx_math/cxx_radian.h
    #vectors
    include/cxx_math/cxx_base_vector.h
    include/cxx_math/cxx_vector2.h
    include/cxx_math/cxx_vector3.h
    include/cxx_math/cxx_vector4.h
    #matrix
    include/cxx_math/cxx_base_matrix.h
    include/cxx_math/cxx_matrix2x2.h
    include/cxx_math/cxx_matrix2x3.h
    include/cxx_math/cxx_matrix3x3.h
    include/cxx_math/cxx_matrix3x4.h
    include/cxx_math/cxx_matrix4x4.h
    #rotation
    include/cxx_math/cxx_euler.h
    include/cxx_math/cxx_quaternion.h
    include/cxx_math/cxx_dual_quaternion.h
    #color
    include/cxx_math/cxx_base_color.h
    include/cxx_math/cxx_color3.h
    include/cxx_math/cxx_color4.h
    #point
    include/cxx_math/cxx_base_point.h
    include/cxx_math/cxx_point2.h
    include/cxx_math/cxx_point3.h
)

set(cxx_utilities_lib_source_files
    source/dummy.cpp
)

set(cxx_utilities_files
    ${cxx_utilities_header_files}
    ${cxx_utilities_math_header_files}
    ${cxx_utilities_math_source_files}
    ${cxx_utilities_lib_source_files}
)

##
#   cxx header list dummy project
#
add_library(cxx_utilities ${cxx_utilities_files})
target_link_libraries(cxx_utilities cxx_library)

source_group("include"          FILES ${cxx_utilities_header_files} )
source_group("include\\math"    FILES ${cxx_utilities_math_header_files} )
source_group("source"           FILES ${cxx_utilities_lib_source_files} ${cxx_source_files})
source_group("source\\math"     FILES ${cxx_utilities_math_source_files})


if(INSTALL_PACKAGE)
    install(DIRECTORY include/ DESTINATION include)
    install(TARGETS cxx_library EXPORT cxx-config DESTINATION lib)
    install(EXPORT cxx-config DESTINATION cmake)
else()
    set(CXX_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/cxx-config.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cxx-config.cmake)
endif()
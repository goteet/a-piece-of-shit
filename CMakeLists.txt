cmake_minimum_required(VERSION 3.12)
#这是有一个总入口，把所有零散的项目组在一起而已
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
project(a-piece-of-trash)

add_subdirectory(task_graph task_graph_demo)
add_subdirectory(cxx_utilities)
add_subdirectory(image_reader)
add_subdirectory(image_loading_test)
include_external_msproject(mock_resource_loading_test 
${CMAKE_CURRENT_LIST_DIR}/mock_resource_loading_test/mock_resource_loading_test.csproj)
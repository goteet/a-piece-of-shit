cmake_minimum_required(VERSION 3.12)
project(image_loading_test)

include_directories(${CMAKE_CURRENT_LIST_DIR})

set(IMAGE_LOADING_TEST_SOURCE_FILES
	../task_graph/mpmc_bounded_queue.h
	../task_graph/task_graph.h
	../task_graph/task_graph.cpp
	winmain.cpp
	test.cpp
)
	
if(MSVC)
	add_executable(image_loading_test WIN32 ${IMAGE_LOADING_TEST_SOURCE_FILES})
	set_target_properties(image_loading_test PROPERTIES COMPILE_DEFINITIONS "UNICODE;_UNICODE")
	set_target_properties(image_loading_test PROPERTIES 
		VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
		#RUNTIME_OUTPUT_DIRECTORY ${CMAKE_LIST_DIR}/bin)
	target_link_libraries(image_loading_test cxx_library image_reader d3d9.lib)
endif(MSVC)
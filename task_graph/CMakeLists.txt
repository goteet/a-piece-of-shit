cmake_minimum_required(VERSION 3.12)

project(task_graph_demo)

add_executable(task_graph_demo mpmc_bounded_queue.h task_graph.cpp)

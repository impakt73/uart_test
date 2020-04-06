cmake_minimum_required(VERSION 3.10)


project(uart_test_project)

add_executable(uart_test)

target_sources(uart_test PRIVATE main.cpp)
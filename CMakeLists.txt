cmake_minimum_required(VERSION 3.20)
project(OrderBook CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(orderbook
    src/main.cpp
)

target_include_directories(orderbook PRIVATE include)
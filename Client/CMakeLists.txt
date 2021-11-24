cmake_minimum_required(VERSION 3.0.0)
project(Client VERSION 0.0.1)

set(SRC_LIST src/main.cpp src/client.cpp)

add_executable(client ${SRC_LIST})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -O0 -Wall")# -I/home/dim/boost_1_77_0 -L/home/dim/boost_1_77_0/stage/lib/")

set(BOOST_ROOT "/home/dim/boost_1_77_0")

find_package(Boost COMPONENTS system regex date_time REQUIRED)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)

include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(client ${Boost_LIBRARIES} -lpthread)
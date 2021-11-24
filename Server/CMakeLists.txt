cmake_minimum_required(VERSION 3.0.0)
project(Server VERSION 0.0.1)

set(SRC_LIST src/main.cpp src/server.cpp src/session.cpp src/database.cpp)

add_executable(server ${SRC_LIST})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -O3 -Wall")# -I/home/dim/boost_1_77_0 -L/home/dim/boost_1_77_0/stage/lib/")

set(BOOST_ROOT "/home/dim/boost_1_77_0")

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost COMPONENTS system regex date_time REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(server ${Boost_LIBRARIES} -lpthread)
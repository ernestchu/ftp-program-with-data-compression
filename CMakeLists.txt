cmake_minimum_required(VERSION 3.8...3.20)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")

# set(CMAKE_BUILD_TYPE Debug)              # defaults to -g
set(CMAKE_BUILD_TYPE Release)            # defaults to -O3 -DNDEBUG
# set(CMAKE_BUILD_TYPE RelWithDebInfo)     # defaults to -O2 -g

project(hw2 LANGUAGES CXX)

add_subdirectory(src)
add_subdirectory(apps)

configure_file(src/Client/Views/HelpMsg.txt exec/HelpMsg.txt)

cmake_minimum_required(VERSION 3.8...3.20)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(hw2 LANGUAGES CXX)

add_subdirectory(src)
add_subdirectory(apps)

configure_file(src/Client/Views/HelpMsg.txt exec/HelpMsg.txt)

cmake_minimum_required(VERSION 3.8...3.20)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

project(ftp-program-with-data-compression LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")

# set(CMAKE_BUILD_TYPE Debug   CACHE STRING "-g"           FORCE)
set(CMAKE_BUILD_TYPE Release CACHE STRING "-O3 -DNDEBUG" FORCE)
# set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "-O2 -g" FORCE)

# for ale
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)
add_subdirectory(apps)

# neeede by the Server to process files
add_custom_target(
    MakeDir
    ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory exec/Data exec/HuffmanCache
    COMMENT "Making Data and HuffmanCache directories needed by the Server"
)

# needed by the Client's help view
add_custom_target(
    HelpMsg
    ALL
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/src/Client/Views/HelpMsg.txt ${CMAKE_BINARY_DIR}/exec/HelpMsg.txt
    COMMENT "Copying HelpMsg.txt needed by the Client"
)

set(HDIR "${CMAKE_SOURCE_DIR}/include/Client")

add_library(CliSrvcLib STATIC Services.cpp ${HDIR}/Services.hpp)

target_include_directories(CliSrvcLib PUBLIC ${HDIR})

target_link_libraries(CliSrvcLib PUBLIC HuffmanLib)

target_compile_features(CliSrvcLib PUBLIC cxx_std_17)

add_subdirectory(Views)

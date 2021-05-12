set(HDIR "${CMAKE_SOURCE_DIR}/include/Server")

add_library(SrvSrvcLib STATIC Services.cpp ${HDIR}/Services.hpp)

target_include_directories(SrvSrvcLib PUBLIC ${HDIR})

target_link_libraries(SrvSrvcLib PUBLIC HuffmanLib)

target_compile_features(SrvSrvcLib PUBLIC cxx_std_17)

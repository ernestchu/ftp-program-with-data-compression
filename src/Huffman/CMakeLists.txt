set(HDIR "${CMAKE_SOURCE_DIR}/include/Huffman")

add_library(HuffmanLib STATIC Huffman.cpp ${HDIR}/Huffman.hpp)

target_include_directories(HuffmanLib PUBLIC ${HDIR})

target_compile_features(HuffmanLib PUBLIC cxx_std_17)

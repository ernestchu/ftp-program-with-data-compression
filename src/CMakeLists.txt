add_library(HuffmanLib STATIC Huffman/Huffman.cpp ../include/Huffman/Huffman.hpp)

add_library(CliSrvcLib STATIC Client/Services.cpp ../include/Client/Services.hpp)
add_library(ViewHelpLib STATIC Client/Views/Help.cpp ../include/Client/Views/Help.hpp)
add_library(ViewHomeLib STATIC Client/Views/Home.cpp ../include/Client/Views/Home.hpp)
add_library(ViewLinkLib STATIC Client/Views/Link.cpp ../include/Client/Views/Link.hpp)
add_library(ViewSendLib STATIC Client/Views/Send.cpp ../include/Client/Views/Send.hpp)

add_library(SrvSrvcLib STATIC Server/Services.cpp ../include/Server/Services.hpp)

target_include_directories(HuffmanLib PUBLIC ../include)
target_include_directories(CliSrvcLib PUBLIC ../include)
target_include_directories(ViewHelpLib PUBLIC ../include)
target_include_directories(ViewHomeLib PUBLIC ../include)
target_include_directories(ViewLinkLib PUBLIC ../include)
target_include_directories(ViewSendLib PUBLIC ../include)
target_include_directories(SrvSrvcLib PUBLIC ../include)

target_link_libraries(CliSrvcLib PUBLIC HuffmanLib)
target_link_libraries(SrvSrvcLib PUBLIC HuffmanLib)

target_compile_features(HuffmanLib PUBLIC cxx_std_17)
target_compile_features(CliSrvcLib PUBLIC cxx_std_17)
target_compile_features(ViewHelpLib PUBLIC cxx_std_17)
target_compile_features(ViewHomeLib PUBLIC cxx_std_17)
target_compile_features(ViewLinkLib PUBLIC cxx_std_17)
target_compile_features(ViewSendLib PUBLIC cxx_std_17)
target_compile_features(SrvSrvcLib PUBLIC cxx_std_17)

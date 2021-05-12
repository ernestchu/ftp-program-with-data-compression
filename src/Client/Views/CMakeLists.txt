set(HDIR "${CMAKE_SOURCE_DIR}/include/Client/Views")

add_library(ViewHelpLib STATIC Help.cpp ${HDIR}/Help.hpp)
add_library(ViewHomeLib STATIC Home.cpp ${HDIR}/Home.hpp)
add_library(ViewLinkLib STATIC Link.cpp ${HDIR}/Link.hpp)
add_library(ViewSendLib STATIC Send.cpp ${HDIR}/Send.hpp)

target_include_directories(ViewHelpLib PUBLIC ${HDIR})
target_include_directories(ViewHomeLib PUBLIC ${HDIR})
target_include_directories(ViewLinkLib PUBLIC ${HDIR})
target_include_directories(ViewSendLib PUBLIC ${HDIR})

target_compile_features(ViewHelpLib PUBLIC cxx_std_17)
target_compile_features(ViewHomeLib PUBLIC cxx_std_17)
target_compile_features(ViewLinkLib PUBLIC cxx_std_17)
target_compile_features(ViewSendLib PUBLIC cxx_std_17)

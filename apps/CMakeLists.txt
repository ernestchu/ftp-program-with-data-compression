find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(Client Client.cpp)
target_link_libraries(Client PUBLIC CliSrvcLib ViewHelpLib ViewHomeLib ViewLinkLib ViewSendLib ${CURSES_LIBRARIES})
target_compile_features(Client PUBLIC cxx_std_17)

add_executable(Server Server.cpp)
target_link_libraries(Server PUBLIC SrvSrvcLib)
target_compile_features(Server PUBLIC cxx_std_17)

add_custom_command(
    TARGET Client 
    POST_BUILD 
    COMMAND "${CMAKE_COMMAND}" -E copy "$<TARGET_FILE:Client>" ../exec/Client
    COMMENT "Copying Client to exec directory"
)
add_custom_command(
    TARGET Server
    POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy "$<TARGET_FILE:Server>" ../exec/Server
    COMMENT "Copying Server to exec directory"
)

add_library(crossing_consoles_lib
        Example.cpp
        communication/Message.h communication/ICommunication.h communication/ClientCommunication.h communication/ClientCommunication.cpp communication/Message.cpp communication/ServerCommunication.h communication/ServerCommunication.cpp communication/ClientConnection.h communication/ClientConnection.cpp)

add_executable(crossing_consoles_main_server
        main_client.cpp)
target_link_libraries(crossing_consoles_main_server crossing_consoles_lib)

add_executable(crossing_consoles_main_client
        main_client.cpp)
target_link_libraries(crossing_consoles_main_client crossing_consoles_lib)

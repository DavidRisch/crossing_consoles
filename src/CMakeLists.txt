add_library(crossing_consoles_lib
        Example.cpp

        game/Player.cpp
        game/Position.cpp
        game/World.cpp
        game/Wall.cpp

        communication/ClientCommunication.cpp
        communication/messages/Message.cpp
        communication/ServerCommunication.cpp
        communication/ClientConnection.cpp
        communication/ServerClientConnection.cpp

        communication/messages/ConnectionRequestMessage.cpp
        communication/messages/ConnectionResponseMessage.cpp
        communication/messages/KeepAliveMessage.cpp
        communication/messages/PayloadMessage.cpp
        communication/messages/MessageMetaData.cpp
        )

add_executable(crossing_consoles_main_server
        main_server.cpp)
target_link_libraries(crossing_consoles_main_server crossing_consoles_lib)

add_executable(crossing_consoles_main_client
        main_client.cpp)
target_link_libraries(crossing_consoles_main_client crossing_consoles_lib)

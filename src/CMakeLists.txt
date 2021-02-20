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

        communication/messages/RequestMessage.cpp
        communication/messages/ResponseMessage.cpp
        communication/messages/StatusMessage.cpp
        communication/messages/GameMessage.cpp
        communication/messages/GameCommandMessage.cpp
        )

add_executable(crossing_consoles_main_server
        main_server.cpp)
target_link_libraries(crossing_consoles_main_server crossing_consoles_lib)

add_executable(crossing_consoles_main_client
        main_client.cpp)
target_link_libraries(crossing_consoles_main_client crossing_consoles_lib)

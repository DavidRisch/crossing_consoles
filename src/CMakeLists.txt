add_library(crossing_consoles_lib

        game/GameClient.cpp
        game/Player.cpp
        game/Position.cpp
        game/World.cpp
        game/Wall.cpp
        game/Renderer.cpp
        game/Compositor.cpp
        game/Sprite.cpp

        game/terminal/RealTerminal.cpp
        game/terminal/MockTerminal.cpp

        communication/byte_layer/byte_stream/SocketByteStream.cpp
        communication/byte_layer/byte_stream/SocketByteServer.cpp
        communication/byte_layer/byte_stream/SocketHolder.cpp
        communication/byte_layer/byte_stream/MockInputStream.cpp
        communication/byte_layer/byte_stream/MockBidirectionalByteStream.cpp

        communication/byte_layer/connection_simulator/ConnectionSimulatorPerfect.cpp
        communication/byte_layer/connection_simulator/ConnectionSimulatorFlakey.cpp

        communication/message_layer/message_stream/MessageInputStream.cpp
        communication/message_layer/message_stream/MessageOutputStream.cpp
        communication/message_layer/message_stream/MessageCoder.cpp

        communication/message_layer/message/Message.cpp
        communication/message_layer/message/ConnectionRequestMessage.cpp
        communication/message_layer/message/ConnectionResponseMessage.cpp
        communication/message_layer/message/KeepAliveMessage.cpp
        communication/message_layer/message/PayloadMessage.cpp
        communication/message_layer/message/MessageMetaData.cpp
        communication/message_layer/message/AcknowledgeMessage.cpp

        communication/message_layer/crc/CRCHandler.cpp

        communication/connection_layer/connection/Connection.cpp
        communication/connection_layer/connection/ConnectionManager.cpp
        communication/connection_layer/connection/ServerSideConnectionManager.cpp
        communication/connection_layer/connection/ClientSideConnectionManager.cpp

        communication/connection_layer/event/Event.cpp
        communication/connection_layer/event/ConnectEvent.cpp
        communication/connection_layer/event/DisconnectEvent.cpp
        communication/connection_layer/event/PayloadEvent.cpp
        communication/message_layer/message/ConnectionResetMessage.cpp
        )

add_executable(crossing_consoles_main_server
        main_server.cpp)
target_link_libraries(crossing_consoles_main_server crossing_consoles_lib)

add_executable(crossing_consoles_main_client
        main_client.cpp)
target_link_libraries(crossing_consoles_main_client crossing_consoles_lib)

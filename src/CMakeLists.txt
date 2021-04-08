add_library(crossing_consoles_lib

        game/GameClient.cpp
        game/GameLogic.cpp
        game/GameServer.cpp

        game/common/Position.cpp
        game/common/Color.cpp

        game/world/Player.cpp
        game/world/World.cpp
        game/world/Wall.cpp
        game/world/WorldGenerator.cpp
        game/world/Spawner.cpp
        game/world/Projectile.cpp

        game/world/items/ItemGenerator.cpp
        game/world/items/Gun.cpp
        game/world/items/Sword.cpp
        game/world/items/Heart.cpp
        game/world/items/Points.cpp

        game/visual/Renderer.cpp
        game/visual/Compositor.cpp
        game/visual/ColoredCharMatrix.cpp
        game/visual/ColoredChar.cpp
        game/visual/ColoredString.cpp
        game/visual/PlayerList.cpp
        game/visual/HealthDisplay.cpp

        game/networking/Change.cpp
        game/networking/SerializationUtils.cpp

        game/terminal/RealTerminal.cpp
        game/terminal/MockTerminal.cpp

        communication/debug.cpp

        communication/byte_layer/byte_stream/SocketByteStream.cpp
        communication/byte_layer/byte_stream/SocketByteServer.cpp
        communication/byte_layer/byte_stream/SocketHolder.cpp
        communication/byte_layer/byte_stream/MockInputStream.cpp
        communication/byte_layer/byte_stream/MockBidirectionalByteStream.cpp

        communication/byte_layer/connection_simulator/ConnectionSimulatorPerfect.cpp
        communication/byte_layer/connection_simulator/ConnectionSimulatorFlaky.cpp
        communication/byte_layer/connection_simulator/PerfectConnectionSimulatorProvider.cpp

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
        communication/message_layer/message/ConnectionResetMessage.cpp
        communication/message_layer/message/NotAcknowledgeMessage.cpp

        communication/message_layer/crc/CRCHandler.cpp

        communication/connection_layer/connection/Connection.cpp
        communication/connection_layer/connection/ConnectionManager.cpp
        communication/connection_layer/connection/ServerSideConnectionManager.cpp
        communication/connection_layer/connection/ClientSideConnectionManager.cpp

        communication/connection_layer/connection/statistics/ConnectionStatistics.cpp
        communication/connection_layer/connection/statistics/StatisticPrinter.cpp

        communication/connection_layer/event/Event.cpp
        communication/connection_layer/event/ConnectEvent.cpp
        communication/connection_layer/event/DisconnectEvent.cpp
        communication/connection_layer/event/PayloadEvent.cpp
        )

add_executable(crossing_consoles_main_server
        main_server.cpp)
target_link_libraries(crossing_consoles_main_server crossing_consoles_lib)
if (WIN32)
    target_link_libraries(crossing_consoles_main_server ws2_32)
endif (WIN32)

add_executable(crossing_consoles_main_client
        main_client.cpp)
target_link_libraries(crossing_consoles_main_client crossing_consoles_lib)
if (WIN32)
    target_link_libraries(crossing_consoles_main_client ws2_32)
endif (WIN32)

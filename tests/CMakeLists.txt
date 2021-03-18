add_executable(crossing_consoles_test
        main.cpp
        byte_stream.cpp
        connection_simulator.cpp
        message_coder.cpp
        crc_handler.cpp
        mocks.cpp
        message_stream.cpp
        connection.cpp
        game.cpp
        connection_manager.cpp
        game_serialization.cpp
        game_networking.cpp
        position.cpp
        sprite.cpp
        renderer.cpp
        wall.cpp
        player.cpp
        world.cpp
        spawner.cpp
        )

target_link_libraries(crossing_consoles_test crossing_consoles_lib)
target_link_libraries(crossing_consoles_test gtest_main)
if (WIN32)
    target_link_libraries(crossing_consoles_test ws2_32)
endif (WIN32)

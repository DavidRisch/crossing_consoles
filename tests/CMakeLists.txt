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
        game_player_list.cpp
        position.cpp
        renderer.cpp
        wall.cpp
        player.cpp
        world.cpp
        colored_char_matrix.cpp
        spawner.cpp
        flaky_connection.cpp
        height_map.cpp
        sprite_map.cpp
        statistics.cpp
        game_logic.cpp
        game_projectile.cpp
        item_generator.cpp
        item.cpp
        colored_fields.cpp
        fixtures/GamePlay.cpp
        utils/detect_debugger.cpp
        utils/TimingHelper.cpp
        )

target_link_libraries(crossing_consoles_test crossing_consoles_lib)
target_link_libraries(crossing_consoles_test gtest_main)
if (WIN32)
    target_link_libraries(crossing_consoles_test ws2_32)
endif (WIN32)

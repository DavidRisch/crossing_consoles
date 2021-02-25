add_executable(crossing_consoles_test
        main.cpp
        byte_stream.cpp
        connection_simulator.cpp
        crc_handler.cpp
        )
target_link_libraries(crossing_consoles_test crossing_consoles_lib)
target_link_libraries(crossing_consoles_test gtest_main)
if (WIN32)
    target_link_libraries(crossing_consoles_test ws2_32)
endif (WIN32)
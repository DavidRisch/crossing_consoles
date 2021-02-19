add_executable(crossing_consoles_test
        main.cpp
        byte_steam.cpp
        )
target_link_libraries(crossing_consoles_test crossing_consoles_lib)
target_link_libraries(crossing_consoles_test gtest_main)

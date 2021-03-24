cmake_minimum_required(VERSION 3.17)

add_custom_target(doc_uml
        COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/build.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        VERBATIM)

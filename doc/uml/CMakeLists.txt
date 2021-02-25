cmake_minimum_required(VERSION 3.17)

add_custom_target(doc_uml
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/build.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        VERBATIM)

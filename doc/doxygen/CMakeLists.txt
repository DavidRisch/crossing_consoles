cmake_minimum_required(VERSION 3.17)

find_package(Doxygen)
if (DOXYGEN_FOUND)
    add_custom_target(doc_doxygen
            COMMAND ${DOXYGEN_EXECUTABLE}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            VERBATIM)
else (DOXYGEN_FOUND)
    message(WARNING "Doxygen not found")
    add_custom_target(doc_doxygen
            COMMAND echo "Doxygen not found, skipping!"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            VERBATIM)
endif (DOXYGEN_FOUND)

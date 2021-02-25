cmake_minimum_required(VERSION 3.17)

ADD_SUBDIRECTORY(doxygen)
ADD_SUBDIRECTORY(uml)

add_custom_target(doc)
add_dependencies(doc
        doc_doxygen
        doc_uml
        )

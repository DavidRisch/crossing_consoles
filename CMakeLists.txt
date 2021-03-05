cmake_minimum_required(VERSION 3.17)

project(crossing_consoles)

set(CMAKE_CXX_STANDARD 20)

if (WIN32)
    find_package(Boost)
    if(Boost_FOUND)
        include_directories(${Boost_INCLUDE_DIRS})
    endif()
endif (WIN32)

add_compile_options(-Wall -Wextra -pedantic)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(gtest)
ADD_SUBDIRECTORY(tests)
ADD_SUBDIRECTORY(doc)

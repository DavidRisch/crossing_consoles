cmake_minimum_required(VERSION 3.17)

project(crossing_consoles)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

add_compile_options(-Wall -Wextra -pedantic)

# treat all warnings as errors
add_compile_options(-Werror)

# uncomment to enable verbose debug output
# add_compile_definitions(CROSSING_CONSOLES_DO_DEBUG)

if (UNIX)
    # Use a unix socket instead of an ip socket (better performance)
    add_compile_definitions(USE_UNIX_SOCKET)
endif (UNIX)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(gtest)
ADD_SUBDIRECTORY(tests)
ADD_SUBDIRECTORY(doc)

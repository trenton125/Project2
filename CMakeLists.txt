cmake_minimum_required(VERSION 3.30)
project(Project2)

set(CMAKE_CXX_STANDARD 14)

# SFML Configuration

include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x
)
FetchContent_MakeAvailable(SFML)

add_executable(Project2
        heap.h
        heap.cpp
        splay.cpp
        splay.h
        json.hpp
        GameDatabaseWindow.h
        GameDatabaseWindow.cpp
        main.cpp
)

target_link_libraries(Project2 PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
)

if(WIN32)
    add_custom_command(
            TARGET Project2
            COMMENT "Copy SFML DLLs"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>
            $<TARGET_FILE_DIR:Project2>
            VERBATIM
    )
endif()
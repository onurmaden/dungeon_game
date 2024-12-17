cmake_minimum_required(VERSION 3.30)
project(dungeon_game C)

set(CMAKE_C_STANDARD 11)

add_executable(dungeon_game main.c
        game.c
        game.h
        player.c
        player.h
        room.c
        room.h
        item.c
        item.h
        creature.c
        creature.h
        commands.c
        commands.h
        save.c
        save.h)

file(COPY ${CMAKE_SOURCE_DIR}/datafiles DESTINATION ${CMAKE_BINARY_DIR})

file(COPY ${CMAKE_SOURCE_DIR}/saves DESTINATION ${CMAKE_BINARY_DIR})

include_directories(${CMAKE_SOURCE_DIR})

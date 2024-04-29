cmake_minimum_required(VERSION 3.23)
project(ConnectFour C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(ConnectFour
        AI.c
        AI.h
        Game.c
        Game.h
        main.c
        Settings.h)

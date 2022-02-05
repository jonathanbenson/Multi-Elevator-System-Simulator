
cmake_minimum_required(VERSION 2.8.9)

project(MESS)

include_directories(include)

set(SOURCES src/client.c src/elevator.c src/priority_queue.c)

add_executable(MESS main.c ${SOURCES})


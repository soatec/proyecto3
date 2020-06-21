cmake_minimum_required(VERSION 3.15)
project(proyecto3 C)

set(CMAKE_C_STANDARD 11)
SET(CMAKE_C_FLAGS -pthread)

include_directories(include)

add_executable(proyecto3
        src/threadville_main.c
        src/user_interface.c include/threadville.h src/user_interface.c include/user_interface.h src/threadville_main.c include/threadville_types.h src/utils.c include/utils.h src/threadville.c)

target_link_libraries(proyecto3 -lSDL2 -lSDL2_image)


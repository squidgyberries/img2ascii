cmake_minimum_required(VERSION 3.0)
project(img2ascii VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(img2ascii src/main.c src/stb_image.h)
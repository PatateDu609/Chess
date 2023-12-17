cmake_minimum_required(VERSION 3.27 FATAL_ERROR)
project(chess LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED COMPONENTS SDL2)
find_package(SDL2_ttf REQUIRED COMPONENTS SDL2_ttf)
find_package(SDL2_image REQUIRED COMPONENTS SDL2_image)

file(GLOB_RECURSE SRC_FILES src/*.cpp include/*.hpp)

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE include PUBLIC /opt/homebrew/opt/llvm/include)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_ttf::SDL2_ttf)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_image::SDL2_image)


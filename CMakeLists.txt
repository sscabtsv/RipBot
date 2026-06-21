cmake_minimum_required(VERSION 3.27.9)
project(RipBot VERSION 1.0.0)
find_package(Geode REQUIRED)

file(GLOB_RECURSE SRC_FILES
    "*.cpp"
    "src/*.cpp"
)

add_library(${PROJECT_NAME} MODULE ${SRC_FILES})
setup_geode_mod(${PROJECT_NAME})

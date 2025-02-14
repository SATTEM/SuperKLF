#这是./src/Managers中的CMakeLists.txt
cmake_minimum_required(VERSION 3.5.0)
project(Managers)
add_subdirectory(${SRC}/Managers/ResourceManager)
add_subdirectory(${SRC}/Managers/GameStage)
add_subdirectory(${SRC}/Managers/Data)
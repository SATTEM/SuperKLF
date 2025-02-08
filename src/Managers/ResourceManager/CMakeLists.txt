#ResourceManager中的CMakeLists.txt
cmake_minimum_required(VERSION 3.5.0)
#每个依赖库的名字
project(ResourceManager)
file(GLOB CURRENT_FILE ${PROJECT_SOURCE_DIR}/*.cpp)
add_library(${PROJECT_NAME} STATIC ${CURRENT_FILE})
target_include_directories(${PROJECT_NAME} PUBLIC ${HEADPATH})

target_link_directories(${PROJECT_NAME} PRIVATE ${LIBPATH})
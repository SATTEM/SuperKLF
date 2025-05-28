#根目录CMakeLists.txt
cmake_minimum_required(VERSION 3.5.0)
project(SuperKLF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SRC ${PROJECT_SOURCE_DIR}/src)
set(LIBPATH ${PROJECT_SOURCE_DIR}/libs)
set(GAMEPATH ${PROJECT_SOURCE_DIR}/bin)
set(HEADPATH ${PROJECT_SOURCE_DIR}/include)
#输出clangd要读取的配置
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#可执行文件的输出路径
set(EXECUTABLE_OUTPUT_PATH ${GAMEPATH})
#动态库输出路径
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIBPATH})
#静态库输出路径
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIBPATH})
file(COPY res DESTINATION ${GAMEPATH}/)
file(COPY config.json DESTINATION ${GAMEPATH}/)
add_compile_definitions(ASSETS_IMAGE_PATH="./res/image/")
add_compile_definitions(ASSETS_DATA_PATH="./res/data/")
#每次添加新子文件夹时都要加入
add_subdirectory(${SRC}/Entity)
add_subdirectory(${SRC}/Tools)
add_subdirectory(${SRC}/UI)
add_subdirectory(${SRC}/SpecialEffect)
add_subdirectory(${SRC}/Managers)
add_subdirectory(${SRC}/RewardSystem)
add_subdirectory(${SRC}/Level)
add_subdirectory(${PROJECT_SOURCE_DIR}/src)

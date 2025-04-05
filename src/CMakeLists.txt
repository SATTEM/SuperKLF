#./src的CMakeLists.txt
cmake_minimum_required(VERSION 3.5.0)
project(SuperKLF_alpha)
file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS ${SRC}/*.cpp)
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${HEADPATH})
#依赖库，每次添加库组件都要在这里加上
#注意，越基础的库越要在后面
target_link_libraries(${PROJECT_NAME} PRIVATE 
	Level
	Entity
	GameStage
	Data
	UI
	RewardSystem
	UIComponent
	SpecialEffect
	ResourceManager
	Tools
	raylib)
target_link_directories(${PROJECT_NAME} PRIVATE ${LIBPATH})
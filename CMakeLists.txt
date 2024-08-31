cmake_minimum_required(VERSION 3.15)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 14)

# 查找SDL2
find_package(SDL2 REQUIRED)

# 包含SDL2头文件的目录
include_directories(${SDL2_INCLUDE_DIRS})

# 添加源文件
add_executable(SnakeGame main.cpp)

# 链接SDL2库
target_link_libraries(SnakeGame ${SDL2_LIBRARIES})

# 设置SDL2路径 (根据Homebrew安装路径)
set(SDL2_PATH /opt/homebrew/Cellar/sdl2/2.30.6)  # 版本号可能会有所不同
include_directories(${SDL2_PATH}/include)
link_directories(${SDL2_PATH}/lib)
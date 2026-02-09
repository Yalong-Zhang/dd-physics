cmake_minimum_required(VERSION 3.16)
project(dd-physics LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-O3)

add_library(timer
  src/timer.cpp
)
target_include_directories(timer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(graph
  src/graph.cpp
)
target_include_directories(graph PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(sis
  src/sis.cpp
)
target_include_directories(sis PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(app
  src/main.cpp
)
target_link_libraries(app PRIVATE timer graph sis)
set_target_properties(app PROPERTIES OUTPUT_NAME dd_physics)

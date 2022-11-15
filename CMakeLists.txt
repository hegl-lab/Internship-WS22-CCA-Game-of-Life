cmake_minimum_required(VERSION 3.22)
project(game_of_life)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(GL_LIBRARY GL GLU X11)

add_library(glad src/glad.c)

file(GLOB glfw-abstraction_SRC "src/glfw-abstraction/*.cpp")
add_library(glfw-abstraction ${glfw-abstraction_SRC})

add_executable(max_resolution src/max_resolution.cpp)
target_link_libraries(max_resolution glfw ${GL_LIBRARY} m glad glfw-abstraction)

add_executable(simple_compute src/game_of_life_simple/compute/main.cpp)
target_link_libraries(simple_compute glfw ${GL_LIBRARY} m glad glfw-abstraction)

add_executable(simple_compute_benchmark src/game_of_life_simple/compute/benchmark.cpp)
target_link_libraries(simple_compute_benchmark glfw ${GL_LIBRARY} m glad glfw-abstraction)

add_executable(simple_fragment src/game_of_life_simple/fragment/main.cpp)
target_link_libraries(simple_fragment glfw ${GL_LIBRARY} m glad glfw-abstraction)

add_executable(simple_fragment_benchmark src/game_of_life_simple/fragment/benchmark.cpp)
target_link_libraries(simple_fragment_benchmark glfw ${GL_LIBRARY} m glad glfw-abstraction)

add_executable(kernel src/game_of_life_kernel/main.cpp)
target_link_libraries(kernel glfw ${GL_LIBRARY} m glad glfw-abstraction)

add_executable(growth src/game_of_life_growth/main.cpp)
target_link_libraries(growth glfw ${GL_LIBRARY} m glad glfw-abstraction)

add_executable(larger_than_life src/game_of_life_larger_than_life/main.cpp)
target_link_libraries(larger_than_life glfw ${GL_LIBRARY} m glad glfw-abstraction)

add_executable(continuous_states src/game_of_life_continuous_states/main.cpp)
target_link_libraries(continuous_states glfw ${GL_LIBRARY} m glad glfw-abstraction)

add_executable(continuous_space src/game_of_life_continuous_space/main.cpp)
target_link_libraries(continuous_space glfw ${GL_LIBRARY} m glad glfw-abstraction)

add_executable(smooth_kernel src/game_of_life_smooth_kernel/main.cpp)
target_link_libraries(smooth_kernel glfw ${GL_LIBRARY} m glad glfw-abstraction)

add_executable(smooth_growth src/game_of_life_smooth_growth/main.cpp)
target_link_libraries(smooth_growth glfw ${GL_LIBRARY} m glad glfw-abstraction)

add_custom_target(copy-shaders ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shaders ${CMAKE_BINARY_DIR}/shaders
        DEPENDS ${MY_TARGET})
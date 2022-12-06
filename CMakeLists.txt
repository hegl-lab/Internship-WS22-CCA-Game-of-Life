cmake_minimum_required(VERSION 3.22)
project(game_of_life)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src/glfw-abstraction)

set(GL_LIBRARY GL GLU X11)

add_library(glad src/glad.c)

file(GLOB glfw-abstraction_SRC "src/glfw-abstraction/*.cpp")
add_library(glfw-abstraction ${glfw-abstraction_SRC})

# link libraries that all targets share to all targets
link_libraries(glfw ${GL_LIBRARY} m glad glfw-abstraction)

# library for fft
file(GLOB fft_SRC "src/fft/*.cpp")
add_library(fft ${fft_SRC})

add_executable(max_resolution src/max_resolution.cpp)

# demos of different GoL versions
add_executable(simple_compute src/game_of_life/simple/compute/main.cpp)
add_executable(simple_compute_benchmark src/game_of_life/simple/compute/benchmark.cpp)
add_executable(simple_fragment src/game_of_life/simple/fragment/main.cpp)
add_executable(simple_fragment_benchmark src/game_of_life/simple/fragment/benchmark.cpp)
add_executable(kernel src/game_of_life/kernel/main.cpp)
add_executable(growth src/game_of_life/growth/main.cpp)
add_executable(larger_than_life src/game_of_life/larger_than_life/main.cpp)
add_executable(continuous_states src/game_of_life/continuous_states/main.cpp)
add_executable(continuous_space src/game_of_life/continuous_space/main.cpp)
add_executable(smooth_kernel src/game_of_life/smooth_kernel/main.cpp)
add_executable(smooth_growth src/game_of_life/smooth_growth/main.cpp)
add_executable(smooth_growth_benchmark src/game_of_life/smooth_growth/benchmark.cpp)
add_executable(fft_smooth_growth src/game_of_life/fft_smooth_growth/main.cpp)
target_link_libraries(fft_smooth_growth fft)
add_executable(fft_smooth_growth_benchmark src/game_of_life/fft_smooth_growth/benchmark.cpp)
target_link_libraries(fft_smooth_growth_benchmark fft)


# copy shader files to binary folder
add_custom_target(copy-shaders ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shaders ${CMAKE_BINARY_DIR}/shaders
        DEPENDS ${MY_TARGET})
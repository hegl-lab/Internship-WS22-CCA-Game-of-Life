#include <thread>
#include "../../glfw-abstraction/Init.h"
#include "../../glfw-abstraction/SimpleComputeShader.h"
#include "../../glfw-abstraction/PassthroughShader.h"
#include "../../glfw-abstraction/Arguments.h"

int width = 100;
int height = 100;
int runs = 10;
bool readable = true;

PassthroughShader passthrough_shader;
SimpleComputeShader compute_shader("shaders/game_of_life_simple/compute/shader.comp");

Texture in_texture;
Texture out_texture;

bool render_loop_call(GLFWwindow *window);

void call_after_glfw_init(GLFWwindow *window);

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cerr << "Expected format: " << argv[0] << " [width] [height] [runs]" << std::endl;
        return 1;
    }
    width = std::stoi(argv[1]);
    height = std::stoi(argv[2]);
    runs = std::stoi(argv[3]);

    if (argc >= 5) readable = !(argv[4][0] == 'n' || argv[4][0] == 'N');

    init<render_loop_call, call_after_glfw_init>(5, 5);
    return 0;
}

bool render_loop_call(GLFWwindow *window) {
    double millis = 0.0;
    for (int i = 0; i < runs; ++i) {
        auto time1 = std::chrono::high_resolution_clock::now();
        compute_shader.use();

        compute_shader.bind_uniform("input_texture", in_texture, 0, GL_READ_ONLY);
        compute_shader.bind_uniform("output_texture", out_texture, 1, GL_WRITE_ONLY);

        compute_shader.dispatch(width, height, 1);
        compute_shader.wait();

        passthrough_shader.use();
        passthrough_shader.render_to_texture(out_texture, in_texture);

        glFinish();

        auto time2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> dt = time2 - time1;
        millis += dt.count();

        glfwSwapBuffers(window);
        glfwPollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    if (readable) std::cout << "compute, resolution = " << width << "x" << height << ", runs = " <<runs << ", t = " << millis << "ms" << std::endl;
    else std::cout << width << '\t' << height << '\t' << runs << '\t' << millis << std::endl;

    return false;
}

void call_after_glfw_init(GLFWwindow *window) {
    compute_shader.init(
            generate_arguments_with_default_marker(
                    Argument<int>{"width", width},
                    Argument<int>{"height", height}
            ));
    passthrough_shader.init_without_arguments();

    in_texture = Texture(width, height, GL_RGBA32F, GL_FLOAT, GL_RGB);
    out_texture = Texture(width, height, GL_RGBA32F, GL_FLOAT, GL_RGB);
    in_texture.init();
    out_texture.init();
}
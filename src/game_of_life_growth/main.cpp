#include <thread>
#include <vector>
#include "../glfw-abstraction/GLFWAbstraction.h"

int width;
int height;
int delay;

PassthroughShader passthrough_shader;
FragmentOnlyShader step_shader("shaders/game_of_life_growth/shader.frag");

Texture in_texture;
Texture out_texture;

bool render_loop_call(GLFWwindow *window);

void call_after_glfw_init(GLFWwindow *window);

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Expected format: " << argv[0] << " width height delay" << std::endl;
        return 1;
    }
    width = std::stoi(argv[1]);
    height = std::stoi(argv[2]);
    delay = std::stoi(argv[3]);

    init<render_loop_call, call_after_glfw_init>(width, height);
    return 0;
}

bool render_loop_call(GLFWwindow *window) {
    step_shader.use();
    step_shader.bind_uniform("texture1", in_texture, 0);
    step_shader.render_to_texture(out_texture);

    passthrough_shader.use();
    passthrough_shader.render_to_texture(out_texture, in_texture);
    passthrough_shader.render_to_window(out_texture);

    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    return true;
}

void init_game() {
    std::vector<float> values(3 * width * height);
    for (float &val: values) val = 0.0;

    auto set_pixel = [&](int x, int y) {
        int cord = 3 * (x * height + y);
        values[cord] = 1.0;
        values[cord + 1] = 1.0;
        values[cord + 2] = 1.0;
    };

    set_pixel(10, 10);
    set_pixel(11, 10);
    set_pixel(11, 11);
    set_pixel(10, 11);
    set_pixel(12, 12);
    set_pixel(13, 12);
    set_pixel(13, 13);
    set_pixel(12, 13);

    set_pixel(51, 50);
    set_pixel(52, 50);
    set_pixel(53, 50);
    set_pixel(54, 50);
    set_pixel(55, 50);
    set_pixel(56, 50);

    set_pixel(50, 49);
    set_pixel(56, 49);
    set_pixel(56, 48);
    set_pixel(55, 47);
    set_pixel(50, 47);

    set_pixel(52, 46);
    set_pixel(53, 46);

    in_texture.set_data(values.data());
}

void call_after_glfw_init(GLFWwindow *window) {
    step_shader.init(
            generate_arguments_with_default_marker(
                    Argument<int>{"width", width},
                    Argument<int>{"height", height}
            ));
    passthrough_shader.init_without_arguments();

    in_texture = Texture(width, height, GL_RGBA32F, GL_FLOAT, GL_RGB);
    out_texture = Texture(width, height, GL_RGBA32F, GL_FLOAT, GL_RGB);
    in_texture.init();
    out_texture.init();

    init_game();
}

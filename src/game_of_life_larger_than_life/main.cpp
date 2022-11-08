#include <thread>
#include <vector>
#include "../glfw-abstraction/GLFWAbstraction.h"

int width;
int height;
int delay;

PassthroughShader passthrough_shader;
FragmentOnlyShader step_shader("shaders/game_of_life_larger_than_life/shader.frag");

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

    std::vector<std::vector<int>> cell = {{0,0,0,0,1,0,0,0,0,0}, {0,0,1,1,1,1,1,0,0,0}, {0,1,1,0,0,1,1,1,1,0}, {1,1,0,0,0,1,1,1,1,1}, {1,0,0,0,0,0,1,1,1,1}, {1,1,0,0,0,0,1,1,1,1}, {1,1,1,0,1,1,1,1,1,1}, {0,1,1,1,1,1,1,1,1,0}, {0,1,1,1,1,1,1,1,0,0}, {0,0,1,1,1,1,1,0,0,0}, {0,0,0,1,1,1,0,0,0,0}};
    for (int x = 0; x < cell.size(); ++x) {
        for (int y = 0; y < cell[0].size(); ++y) {
            if (cell[x][y] != 0) {
                set_pixel(x + 30, y + 30);
            }
        }
    }

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

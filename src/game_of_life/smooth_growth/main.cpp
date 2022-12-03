#include <thread>
#include <vector>
#include <random>
#include <cstring>
#include <GLFWAbstraction.h>

int width;
int height;
int delay;
float frequency;
int R;
bool orbium = false;
float m = 0.135;
float s = 0.015;

PassthroughShader passthrough_shader;
FragmentOnlyShader step_shader("shaders/smooth_growth/shader.frag");
FragmentOnlyShader hue_rotation("shaders/hue_rotation.frag");

Texture in_texture;
Texture out_texture;

Texture kernel;

bool render_loop_call(GLFWwindow *window);

void call_after_glfw_init(GLFWwindow *window);

int main(int argc, char *argv[]) {
    if (argc != 6) {
        std::cerr << "Expected format: " << argv[0] << " width height delay frequency R|orbium" << std::endl;
        return 1;
    }
    width = std::stoi(argv[1]);
    height = std::stoi(argv[2]);
    delay = std::stoi(argv[3]);
    frequency = std::stof(argv[4]);
    if (std::strcmp(argv[5], "orbium") == 0) {
        R = 13;
        orbium = true;
    } else {
        R = std::stoi(argv[5]);
    }

    init<render_loop_call, call_after_glfw_init>(500, 500);
    return 0;
}

bool render_loop_call(GLFWwindow *window) {
    step_shader.use();
    step_shader.bind_uniform("texture1", in_texture, 0);
    step_shader.bind_uniform("kernel", kernel, 1);
    step_shader.render_to_texture(out_texture);

    passthrough_shader.use();
    passthrough_shader.render_to_texture(out_texture, in_texture);

    hue_rotation.use();
    hue_rotation.bind_uniform("texture1", out_texture, 0);
    hue_rotation.render_to_window();

    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    return true;
}

void init_game() {
    std::vector<float> values(3 * width * height);
    for (float &val: values) val = 0.0;

    auto set_pixel = [&](int x, int y, float value) {
        int cord = (x * height + y);
        values[cord] = value;
        //values[cord + 1] = value;
        //values[cord + 2] = value;
    };

    if (orbium) {
        // insert orbium into the world
        std::array<std::array<double, 20>, 20> data = {
                std::array<double, 20>{0, 0, 0, 0, 0, 0, 0.1, 0.14, 0.1, 0, 0, 0.03, 0.03, 0, 0, 0.3, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0.08, 0.24, 0.3, 0.3, 0.18, 0.14, 0.15, 0.16, 0.15, 0.09, 0.2, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0.15, 0.34, 0.44, 0.46, 0.38, 0.18, 0.14, 0.11, 0.13, 0.19, 0.18, 0.45, 0, 0, 0},
                {0, 0, 0, 0, 0.06, 0.13, 0.39, 0.5, 0.5, 0.37, 0.06, 0, 0, 0, 0.02, 0.16, 0.68, 0, 0, 0},
                {0, 0, 0, 0.11, 0.17, 0.17, 0.33, 0.4, 0.38, 0.28, 0.14, 0, 0, 0, 0, 0, 0.18, 0.42, 0, 0},
                {0, 0, 0.09, 0.18, 0.13, 0.06, 0.08, 0.26, 0.32, 0.32, 0.27, 0, 0, 0, 0, 0, 0, 0.82, 0, 0},
                {0.27, 0, 0.16, 0.12, 0, 0, 0, 0.25, 0.38, 0.44, 0.45, 0.34, 0, 0, 0, 0, 0, 0.22, 0.17, 0},
                {0, 0.07, 0.2, 0.02, 0, 0, 0, 0.31, 0.48, 0.57, 0.6, 0.57, 0, 0, 0, 0, 0, 0, 0.49, 0},
                {0, 0.59, 0.19, 0, 0, 0, 0, 0.2, 0.57, 0.69, 0.76, 0.76, 0.49, 0, 0, 0, 0, 0, 0.36, 0},
                {0, 0.58, 0.19, 0, 0, 0, 0, 0, 0.67, 0.83, 0.9, 0.92, 0.87, 0.12, 0, 0, 0, 0, 0.22, 0.07},
                {0, 0, 0.46, 0, 0, 0, 0, 0, 0.7, 0.93, 1, 1, 1, 0.61, 0, 0, 0, 0, 0.18, 0.11},
                {0, 0, 0.82, 0, 0, 0, 0, 0, 0.47, 1, 1, 0.98, 1, 0.96, 0.27, 0, 0, 0, 0.19, 0.1},
                {0, 0, 0.46, 0, 0, 0, 0, 0, 0.25, 1, 1, 0.84, 0.92, 0.97, 0.54, 0.14, 0.04, 0.1, 0.21, 0.05},
                {0, 0, 0, 0.4, 0, 0, 0, 0, 0.09, 0.8, 1, 0.82, 0.8, 0.85, 0.63, 0.31, 0.18, 0.19, 0.2, 0.01},
                {0, 0, 0, 0.36, 0.1, 0, 0, 0, 0.05, 0.54, 0.86, 0.79, 0.74, 0.72, 0.6, 0.39, 0.28, 0.24, 0.13, 0},
                {0, 0, 0, 0.01, 0.3, 0.07, 0, 0, 0.08, 0.36, 0.64, 0.7, 0.64, 0.6, 0.51, 0.39, 0.29, 0.19, 0.04, 0},
                {0, 0, 0, 0, 0.1, 0.24, 0.14, 0.1, 0.15, 0.29, 0.45, 0.53, 0.52, 0.46, 0.4, 0.31, 0.21, 0.08, 0, 0},
                {0, 0, 0, 0, 0, 0.08, 0.21, 0.21, 0.22, 0.29, 0.36, 0.39, 0.37, 0.33, 0.26, 0.18, 0.09, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0.03, 0.13, 0.19, 0.22, 0.24, 0.24, 0.23, 0.18, 0.13, 0.05, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0.02, 0.06, 0.08, 0.09, 0.07, 0.05, 0.01, 0, 0, 0, 0, 0}};

        auto insert_orbium = [&](int dx, int dy) {
            for (int x = 0; x < 20; ++x) {
                for (int y = 0; y < 20; ++y) {
                    set_pixel(x + dx, y + dy, data[x][y]);
                }
            }
        };

        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<int> distribution_x(0, width - 20);
        std::uniform_int_distribution<int> distribution_y(0, height - 20);

        for (int x = 0; x < 1; ++x) {
            insert_orbium(distribution_x(rng), distribution_y(rng));
        }
    } else {
        // generate random values to fill the grid
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> distribution(0, 1);
        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                set_pixel(x, y, distribution(rng));
            }
        }
    }

    in_texture.set_data(values.data());
}

void call_after_glfw_init(GLFWwindow *window) {
    step_shader.init(
            generate_arguments_with_default_marker(
                    Argument<int>{"width", width},
                    Argument<int>{"height", height},
                    Argument<float>{"frequency", frequency},
                    Argument<int>{"R", R},
                    Argument<float>{"m", m},
                    Argument<float>{"s", s}
            ));
    passthrough_shader.init_without_arguments();
    hue_rotation.init_without_arguments();

    in_texture = Texture(width, height, GL_R32F, GL_FLOAT, GL_RED);
    out_texture = Texture(width, height, GL_R32F, GL_FLOAT, GL_RED);
    in_texture.init();
    out_texture.init();

    // initialize kernel_data
    auto *kernel_data = new float[(2 * R + 1) * (2 * R + 1)];
    //
    auto bell_function = [](double value, double m = 0.5, double s = 0.15) {
        return std::exp(-std::pow((value - m) / s, 2) / 2);
    };
    // set kernel values based on distance
    for (int x = 0; x < 2 * R + 1; ++x) {
        for (int y = 0; y < 2 * R + 1; ++y) {
            double distance = std::sqrt(std::pow((x - R + 1), 2) + std::pow(y - R + 1, 2));
            kernel_data[x * (2 * R + 1) + y] = float(bell_function((distance) / R));
        }
    }
    // scale kernel_data
    float sum = 0.0;
    for (int x = 0; x < (2 * R + 1) * (2 * R + 1); ++x) sum += kernel_data[x];
    for (int x = 0; x < (2 * R + 1) * (2 * R + 1); ++x) kernel_data[x] /= sum;

    kernel_data[R * (2 * R + 1) + R] = 0.0f;

    kernel = Texture(2 * R + 1, 2 * R + 1, GL_R32F, GL_FLOAT, GL_RED);
    kernel.init();
    kernel.set_data(kernel_data);

    delete[] kernel_data;

    init_game();
}

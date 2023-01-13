#include <thread>
#include <vector>
#include <random>
#include <cstring>
#include <GLFWAbstraction.h>
#include "../../fft/IFFT2D.h"

int width;
int height;
int delay;
int runs;
float frequency;
int R;
bool orbium = false;
float m = 0.135;
float s = 0.015;

PassthroughShader passthrough_shader;
FragmentOnlyShader growth_shader("shaders/game_of_life/fft_smooth_growth/growth.frag");
FragmentOnlyShader product_shader("shaders/game_of_life/fft_smooth_growth/product.frag");
FragmentOnlyShader hue_rotation("shaders/hue_rotation.frag");
IFFT2D ifft2D;

Texture current_state;
Texture previous_state;
Texture update_state;

Texture kernel;

bool render_loop_call(GLFWwindow *window);

void call_after_glfw_init(GLFWwindow *window);

int main(int argc, char *argv[]) {
    if (argc != 5) {
        std::cerr << "Expected format: " << argv[0] << " width height runs R|dynamic" << std::endl;
        return 1;
    }
    width = std::stoi(argv[1]);
    height = std::stoi(argv[2]);
    runs = std::stoi(argv[3]);

    delay = 0;
    frequency = 10;
    if (std::strcmp(argv[4], "dynamic") == 0) R = width / 2 - 1;
    else R = std::stoi(argv[4]);
    orbium = true;

    init<render_loop_call, call_after_glfw_init>(10, 10);
    return 0;
}

int run = 0;
long measurement = 0;

bool render_loop_call(GLFWwindow *window) {
    if (run == runs) {
        std::cout << width << '\t' << measurement << std::endl;
        return false;
    }
    ++run;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    // copy current state, since we will need again later and the content of one of the
    // texture will be replaced with the fourier transformation
    passthrough_shader.use();
    passthrough_shader.render_to_texture(current_state, previous_state);

    // calculate F(current_state)
    ifft2D.fft2D.compute(current_state);

    // calculate F(current_state) * F(K) where * is the scalar product
    product_shader.use();
    product_shader.bind_uniform("G", current_state, 0);
    product_shader.bind_uniform("K", kernel, 1);
    product_shader.render_to_texture(update_state);

    // calculate update_state = F^-1(F(current_state) * F(K)) which equals the update rate
    ifft2D.compute_inverse(update_state);

    // calculate current_state = previous_state + growth(update_state)
    growth_shader.use();
    growth_shader.bind_uniform("source_state", previous_state, 0);
    growth_shader.bind_uniform("update", update_state, 1);
    growth_shader.render_to_texture(current_state);
    glFinish();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    measurement += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

    return true;
}

void init_game() {
    std::vector<float> values(2 * width * height);
    for (float &val: values) val = 0.0;

    auto set_pixel = [&](int x, int y, float value) {
        int cord = 2 * (x * height + y);
        values[cord] = value;
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

        insert_orbium(width / 2 - 10, height / 2 - 10);
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

    current_state.set_data(values.data());
}

void call_after_glfw_init(GLFWwindow *window) {
    growth_shader.init(
            generate_arguments_with_default_marker(
                    Argument<float>{"m", m},
                    Argument<float>{"s", s},
                    Argument<float>{"frequency", frequency}
            ));
    product_shader.init(
            generate_arguments_with_default_marker(
                    Argument<int>{"width", width},
                    Argument<int>{"height", height}
            )
    );
    passthrough_shader.init_without_arguments();
    hue_rotation.init_without_arguments();

    current_state = Texture(width, height, GL_RG32F, GL_FLOAT, GL_RG);
    update_state = Texture(width, height, GL_RG32F, GL_FLOAT, GL_RG);
    previous_state = Texture(width, height, GL_RG32F, GL_FLOAT, GL_RG);
    current_state.init();
    update_state.init();
    previous_state.init();

    // initialize kernel_data
    std::vector<float> kernel_data(2 * width * height);
    for (int i = 0; i < 2 * width * height; ++i) kernel_data[i] = 0.0;
    auto bell_function = [](double value, double m = 0.5, double s = 0.15) {
        return std::exp(-std::pow((value - m) / s, 2) / 2);
    };
    int x_center = width / 2;
    int y_center = height / 2;

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            double distance = std::sqrt(std::pow(x - x_center, 2) + std::pow(y - y_center, 2)) / R;
            int pos = 2 * (x * height + y);
            if (distance < 1.0) kernel_data[pos] = float(bell_function(distance));
        }
    }

    // switch around quadrants
    for (int x = 0; x < width / 2; ++x) {
        for (int y = 0; y < height / 2; ++y) {
            int pos1 = 2 * (x * height + y);
            int pos2 = 2 * (x * height + y + width * height / 2 + height / 2);

            std::swap(kernel_data[pos1], kernel_data[pos2]);

            int pos3 = 2 * (x * height + y + width / 2 * height);
            int pos4 = 2 * (x * height + y + height / 2);

            std::swap(kernel_data[pos3], kernel_data[pos4]);
        }
    }

    // scale kernel_data
    double sum = 0.0;
    for (int x = 0; x < 2 * width * height; ++x) sum += kernel_data[x];
    for (int x = 0; x < 2 * width * height; ++x) kernel_data[x] /= sum;

    kernel = Texture(width, height, GL_RG32F, GL_FLOAT, GL_RG);
    kernel.init();
    kernel.set_data(kernel_data.data());

    ifft2D.init_without_arguments();

    ifft2D.fft2D.compute(kernel);

    init_game();
}

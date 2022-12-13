#include <thread>
#include <vector>
#include <random>
#include <cstring>
#include <GLFWAbstraction.h>
#include "../../fft/GLFFT2D.h"

//GLFFT2D fft_real_forward;
GLFFT2D fft_complex_forward;
GLFFT2D fft_inverse;
Buffer current_state;
Buffer fourier_current;
Buffer update;
Buffer kernel;
Buffer fourier_kernel;
Texture render_texture;

SimpleComputeShader buffer_to_texture("shaders/buffer_to_texture.comp");
SimpleComputeShader buffer_product("shaders/game_of_life/glfft_smooth_growth/buffer_product.comp");
SimpleComputeShader buffer_growth("shaders/game_of_life/glfft_smooth_growth/buffer_growth.comp");
FragmentOnlyShader hue_rotation("shaders/hue_rotation.frag");

int width;
int height;
int delay;
float frequency;
int R;
bool orbium = false;
float m = 0.135;
float s = 0.015;


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

    init<render_loop_call, call_after_glfw_init>(width, height);
    return 0;
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

    /*for (int i = 0; i < width * height; ++i) {
        values[2 * i] = 1.0;
    }*/

    current_state.set_data(values);
}

void init_kernel() {
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

    //for (int i = 0; i < 2 * width * height; i += 8) kernel_data[i] = 1.0;

    kernel.set_data(kernel_data);
}

bool render_loop_call(GLFWwindow *window) {
    // calculate fourier_current = F(current_state)
    fft_complex_forward.compute(current_state, fourier_current);

    // calculate the scalar product of fourier_current and kernel
    buffer_product.use();
    buffer_product.bind_buffer("A", fourier_current, 0);
    buffer_product.bind_buffer("B", kernel, 1);
    buffer_product.dispatch(width * height / 1024, 1, 1);
    buffer_product.wait();

    fft_inverse.compute(fourier_current, fourier_current);

    buffer_growth.use();
    buffer_growth.bind_buffer("A", fourier_current, 0);
    buffer_growth.bind_buffer("B", current_state, 1);
    buffer_growth.dispatch(width * height / 1024, 1, 1);
    buffer_growth.wait();
    //std::cout << fourier_current.get_data()[10] << std::endl;

    buffer_to_texture.use();
    buffer_to_texture.bind_uniform("A", render_texture, 0, GL_WRITE_ONLY);
    buffer_to_texture.bind_buffer("shader_data", current_state, 1);
    buffer_to_texture.bind_uniform("width", width);
    buffer_to_texture.dispatch(width, height, 1);
    buffer_to_texture.wait();

    hue_rotation.use();
    hue_rotation.bind_uniform("texture1", render_texture, 0);
    hue_rotation.render_to_window();

    return true;
}

void call_after_glfw_init(GLFWwindow *window) {
    fft_complex_forward.init(width, height, false, false, GLFFT::ComplexToComplex);
    fft_inverse.init(width, height, true, true, GLFFT::ComplexToComplex);

    current_state = Buffer(2 * width * height);
    current_state.init();

    fourier_current = Buffer(2 * width * height);
    fourier_current.init();

    update = Buffer(2 * width * height);
    update.init();

    kernel = Buffer(2 * width * height);
    kernel.init();
    fourier_kernel = Buffer(2 * width * height);
    fourier_kernel.init();
    init_kernel();
    fft_complex_forward.compute(kernel, kernel);

    buffer_to_texture.init_without_arguments();
    buffer_product.init_without_arguments();
    buffer_growth.init_without_arguments();
    hue_rotation.init_without_arguments();

    render_texture = Texture(width, height, GL_R32F, GL_FLOAT, GL_RED);
    render_texture.init();

    init_game();
}

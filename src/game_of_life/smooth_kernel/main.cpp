#include <thread>
#include <vector>
#include <random>
#include <GLFWAbstraction.h>

int width;
int height;
int delay;
float frequency;
int R;

PassthroughShader passthrough_shader;
FragmentOnlyShader step_shader("shaders/game_of_life/continuous_space/shader.frag");

Texture in_texture;
Texture out_texture;

Texture kernel;

bool render_loop_call(GLFWwindow *window);

void call_after_glfw_init(GLFWwindow *window);

int main(int argc, char *argv[]) {
    if (argc != 6) {
        std::cerr << "Expected format: " << argv[0] << " width height delay frequency R" << std::endl;
        return 1;
    }
    width = std::stoi(argv[1]);
    height = std::stoi(argv[2]);
    delay = std::stoi(argv[3]);
    frequency = std::stof(argv[4]);
    R = std::stoi(argv[5]);

    init<render_loop_call, call_after_glfw_init>(width, height);
    return 0;
}

bool render_loop_call(GLFWwindow *window) {
    step_shader.use();
    step_shader.bind_uniform("texture1", in_texture, 0);
    step_shader.bind_uniform("kernel", kernel, 1);
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

    auto set_pixel = [&](int x, int y, float value) {
        int cord = 3 * (x * height + y);
        values[cord] = value;
        values[cord + 1] = value;
        values[cord + 2] = value;
    };

    // generate random values to fill the grid
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distribution(0, 1);
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            set_pixel(x, y, distribution(rng));
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
                    Argument<int>{"R", R}
            ));
    passthrough_shader.init_without_arguments();

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
            double distance = std::sqrt(std::pow(x - R, 2) + std::pow(y - R, 2));
            kernel_data[x * (2 * R + 1) + y] = float(bell_function(distance / (R + 1)));
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

    //step_shader.use();
    //step_shader.bind_uniform("kernel_data", kernel_data, (2 * R + 1) * (2 * R + 1));

    init_game();
}

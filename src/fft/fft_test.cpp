#include <GLFWAbstraction.h>
#include "FFT1D.h"
#include "FFT2D.h"
#include "IFFT2D.h"
#include "GLFFT2D.h"

bool render_loop_call(GLFWwindow *window);

void call_after_glfw_init(GLFWwindow *window);

GLFFT2D fft;
Texture A(16, 16, GL_RG32F, GL_FLOAT, GL_RG);
Texture B(16, 16, GL_RG32F, GL_FLOAT, GL_RG);

int main() {
    init<render_loop_call, call_after_glfw_init>(1, 1);
}

bool render_loop_call(GLFWwindow *window) {
    //fft.compute(A, B);
    //fft.compute_inverse(A);

    std::vector<float> data = B.get_data<float, 2>();
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            std::cout << data[2 * j + 8 * i] << ',' << data[2 * j + 8 * i + 1] << '\t';
        }
        std::cout << std::endl;
    }

    return false;
}

void call_after_glfw_init(GLFWwindow *window) {
    //fft.init(16, 16, true);
    A.init();
    B.init();

    std::vector<float> values{
            3.0, 0.0, 2.0, 0.0, 5.0, 0.0, 6.0, 0.0,
            7.0, 0.0, 1.0, 0.0, 5.0, 0.0, 2.0, 0.0,
            -5.0, 0.0, 8.0, 0.0, 9.0, 0.0, 0.0, 0.0,
            2.0, 0.0, 3.0, 0.0, 4.0, 0.0, 1.0, 0.0
    };
    //std::vector<float> values{0.0, 0.0, 1.0, 0.0, 2.0, 0.0, 3.0, 0.0, 4.0, 0.0, 5.0, 0.0, 6.0, 0.0, 7.0, 0.0};
    A.set_data(values.data());
}
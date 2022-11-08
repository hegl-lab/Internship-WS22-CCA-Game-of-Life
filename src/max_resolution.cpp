#include "glfw-abstraction/Init.h"

bool render_loop_call(GLFWwindow *window) {
    return false;
}

void call_after_glfw_init(GLFWwindow *window) {
    int value;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);
    std::cout << "Max Texture dimension " << value << '\n';
}

int main() {
    init<render_loop_call, call_after_glfw_init>(5, 5);
}
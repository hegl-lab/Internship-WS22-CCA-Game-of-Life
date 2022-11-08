#ifndef SLIME_SIMULATION_INIT_H
#define SLIME_SIMULATION_INIT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

extern int CURRENT_WIDTH, CURRENT_HEIGHT;

static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
static void processInput(GLFWwindow *window);

/**
 * Called to initialize glfw and create a window to draw to.
 * @tparam render_loop_call function that gets called after every sucesfull render,
 * @tparam call_after_glfw_init function that gets called after glfw has been initialized and a window has been created
 * @param width initial width of the window
 * @param height initial height of the window
 */
template<bool (*render_loop_call)(GLFWwindow *window), void (*call_after_glfw_init)(GLFWwindow *window)>
void init(int width, int height) {
    CURRENT_WIDTH = width;
    CURRENT_HEIGHT = height;

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // create window
    GLFWwindow *window = glfwCreateWindow(width, height, "ComputeParticle shader", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

    call_after_glfw_init(window);

    bool result = true;
    while (!glfwWindowShouldClose(window) && result) {
        processInput(window);

        result = render_loop_call(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

// checks if the ESC-key is pressed and closes the window if it is
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// is called whenever the window is resized
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    CURRENT_WIDTH = width;
    CURRENT_HEIGHT = height;
}

#endif //SLIME_SIMULATION_INIT_H

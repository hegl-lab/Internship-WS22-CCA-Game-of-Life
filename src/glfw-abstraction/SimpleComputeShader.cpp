#include "SimpleComputeShader.h"

#include <glad/glad.h>

SimpleComputeShader::SimpleComputeShader() : id(0), path("") {}

SimpleComputeShader::SimpleComputeShader(const char *path) :id(0), path(path) {}

void SimpleComputeShader::init(const std::string &arguments) {
    // read shader code from file
    std::string compute_code;
    std::ifstream file;

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        file.open(path);
        std::stringstream file_stream;
        file_stream << file.rdbuf();
        file.close();

        compute_code = file_stream.str();

        // insert argument into shader code after version line (=first line)
        std::string first_line = compute_code.substr(0, compute_code.find('\n') + 1);
        compute_code.erase(0, compute_code.find('\n') + 1);
        compute_code = first_line + arguments + compute_code;
    } catch (const std::ifstream::failure &e) {
        std::cerr << "failed to read ComputeParticle shader file" << std::endl;
    }

    const char *c_shader_code = compute_code.c_str();

    // compile shader
    unsigned int shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(shader, 1, &c_shader_code, NULL);
    glCompileShader(shader);

    // check for compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infoLog << std::endl;
        for (int i = 1; compute_code.length() > 0; ++i) {
            auto position = compute_code.find('\n');
            std::string line = std::to_string(i) + "\t" + compute_code.substr(0, position);
            if (position >= 0) compute_code.erase(0, position + 1);
            else compute_code = "";
            std::cerr << line << std::endl;
        }
    }

    // create program
    id = glCreateProgram();
    glAttachShader(id, shader);
    glLinkProgram(id);

    // cleanup
    glDeleteShader(shader);
}

void SimpleComputeShader::init_without_arguments() {
    init("");
}

void SimpleComputeShader::use() const {
    glUseProgram(id);
}

void SimpleComputeShader::dispatch(unsigned int size_x, unsigned int size_y, unsigned int size_z) const {
    glDispatchCompute(size_x, size_y, size_z);
}

void SimpleComputeShader::wait() const {
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
}

void SimpleComputeShader::bind_uniform(const char *name, const Texture &texture, int unit, int access_mode) const {
    GLint location = glGetUniformLocation(id, name);
    glUniform1i(location, unit);
    texture.bind_compute(unit, access_mode);
}

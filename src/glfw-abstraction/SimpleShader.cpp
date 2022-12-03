#include "SimpleShader.h"

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

SimpleShader::SimpleShader() : id(0), vertexPath(""), fragmentPath("") {}


SimpleShader::SimpleShader(const char *vertexPath, const char *fragmentPath) : vertexPath(vertexPath),
                                                                               fragmentPath(fragmentPath), id(0) {}

void SimpleShader::init(const std::string &arguments) {
    // read shader codes from the files
    std::string vertex_code;
    std::string fragment_code;
    std::ifstream v_shader_file;
    std::ifstream f_shader_file;

    v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        v_shader_file.open(vertexPath);
        f_shader_file.open(fragmentPath);
        std::stringstream v_shader_stream, f_shader_stream;
        v_shader_stream << v_shader_file.rdbuf();
        f_shader_stream << f_shader_file.rdbuf();

        v_shader_file.close();
        f_shader_file.close();

        vertex_code = v_shader_stream.str();
        fragment_code = f_shader_stream.str();

        // insert argument into shader code after version line (=first line)
        std::string first_line = vertex_code.substr(0, vertex_code.find('\n') + 1);
        vertex_code.erase(0, vertex_code.find('\n') + 1);
        vertex_code = first_line + arguments + vertex_code;

        first_line = fragment_code.substr(0, fragment_code.find('\n') + 1);
        fragment_code.erase(0, fragment_code.find('\n') + 1);
        fragment_code = first_line + arguments + fragment_code;
    } catch (const std::ifstream::failure &e) {
        std::cerr << "failed to read shader files" << std::endl;
    }

    const char *v_shader_code = vertex_code.c_str();
    const char *f_shader_code = fragment_code.c_str();

    // compile shaders
    unsigned int vertex, fragment;

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &v_shader_code, NULL);
    glCompileShader(vertex);

    // check for compile errors while compiling the vertex shader
    int success;
    char infoLog[512];
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infoLog << std::endl;
        for (int i = 1; vertex_code.length() > 0; ++i) {
            int position = vertex_code.find("\n");
            std::string line = std::to_string(i) + "\t" + vertex_code.substr(0, position);
            if (position >= 0) vertex_code.erase(0, position + 1);
            else vertex_code = "";
            std::cerr << line << std::endl;
        }
    }

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &f_shader_code, NULL);
    glCompileShader(fragment);

    // check for compile errors while compiling the fragment shader
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infoLog << std::endl;
        for (int i = 1; fragment_code.length() > 0; ++i) {
            int position = fragment_code.find("\n");
            std::string line = std::to_string(i) + "\t" + fragment_code.substr(0, position);
            if (position >= 0) fragment_code.erase(0, position + 1);
            else fragment_code = "";
            std::cerr << line << std::endl;
        }
    }

    // create program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);

    // cleanup
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void SimpleShader::init_without_arguments() {
    init("");
}

void SimpleShader::use() const {
    glUseProgram(id);
}

void SimpleShader::bind_uniform(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int) value);
}

void SimpleShader::bind_uniform(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void SimpleShader::bind_uniform(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void SimpleShader::bind_uniform(const char *name, const Texture &texture, int unit) const {
    GLint location = glGetUniformLocation(id, name);
    glUniform1i(location, unit);
    texture.bind(unit);
}

void SimpleShader::bind_uniform(const std::string &name, float *value, int count) const {
    glUniform1fv(glGetUniformLocation(id, name.c_str()), count, value);
}

GLint SimpleShader::get_location(const std::string &name) const {
    return glGetUniformLocation(id, name.c_str());
}

void SimpleShader::bind_uniform(GLint  location, const Texture &texture, int unit) const {
    glUniform1i(location, unit);
    texture.bind(unit);
}

void SimpleShader::bind_uniform(GLint location, bool value) const {
    glUniform1i(location, (int) value);
}

void SimpleShader::bind_uniform(GLint location, int value) const {
    glUniform1i(location, value);
}

void SimpleShader::bind_uniform(GLint location, float value) const {
    glUniform1f(location, value);
}

void SimpleShader::bind_uniform(GLint location, float *value, int count) const {
    glUniform1fv(location, count, value);
}

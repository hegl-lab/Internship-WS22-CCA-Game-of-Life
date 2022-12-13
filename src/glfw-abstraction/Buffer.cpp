#include <cstring>
#include "Buffer.h"

Buffer::Buffer() {}

Buffer::Buffer(int size) : size(size) {}

void Buffer::init() {
    glGenBuffers(1, &id);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * size, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Buffer::set_data(const std::vector<float> &data) {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
    GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
    memcpy(p, data.data(), sizeof(float) * size);
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

std::vector<float> Buffer::get_data() const {
    std::vector<float> data(size);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size * sizeof(float), data.data());
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    return data;
}

void Buffer::bind(int index) const {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, id);
}




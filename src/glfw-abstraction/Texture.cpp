#include "Texture.h"

#include <glad/glad.h>
#include <stddef.h>

Texture::Texture() : id(0), width(-1), height(-1), value_type(-1), mode(-1), type(-1) {}

Texture::Texture(int width, int height, int type, unsigned int value_type, unsigned int mode) : width(width),
                                                                                                height(height),
                                                                                                type(type),
                                                                                                value_type(value_type),
                                                                                                mode(mode), id(-1) {}

void Texture::init() {
    glGenTextures(1, &id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, mode, value_type, NULL);
}

Texture::operator unsigned int() const {
    return id;
}

void Texture::bind(unsigned int unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::bind_compute(int unit, int access_mode) const {
    glBindImageTexture(unit, id, 0, GL_FALSE, 0, access_mode, type);
}
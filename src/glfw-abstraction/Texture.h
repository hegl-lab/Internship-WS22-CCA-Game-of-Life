#ifndef GAME_OF_LIFE_TEXTURE_H
#define GAME_OF_LIFE_TEXTURE_H

#include <vector>
#include <glad/glad.h>

class Texture {
public:
    unsigned int id;
    int width, height;

    Texture();

    /**
     * Texture on the gpu.
     * @param width width ot the texture in px
     * @param height height of the texture in px
     * @param type type of each individual pixel (eg. GL_RGBA32F, GL_RGB32F or GL_RUI)
     * @param value_type type used to store the individual values of each pixel (eg. GL_FLOAT, GL_UNSIGNED_INT)
     * @param mode see type, just with out the value_type (eg. GL_RGB)
     * @param access_mode set if the texture should be read only, write only or both (eg. GL_READ_WRITE, GL_READ, GL_WRITE)
     */
    Texture(int width, int height, int type, unsigned int value_type, unsigned int mode);

    /**
     * Actually creates the texture. MUST be called after glfw has been initialized.
     */
    void init();

    /**
     * Cast operator, used so a texture can be directly used in OpenGL functions
     * @return id of the texture
     */
    operator unsigned int() const;

    // bind texture to a texture unit
    void bind(unsigned int unit) const;

    // binds a texture to an image unit
    void bind_compute(int unit, int access_mode) const;

    // returns the data of a texture
    template<typename val, int vals_per_pixel>
    std::vector<val> get_data() {
        std::vector<val> data(width * height * vals_per_pixel);
        bind(0);
        glGetTexImage(GL_TEXTURE_2D, 0, mode, value_type, data.data());
        return data;
    }

    // sets the data of texture
    template<typename val>
    void set_data(val *values) {
        bind(0);
        glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, mode, value_type, values);
    }

private:
    int value_type, mode, type;
};


#endif //GAME_OF_LIFE_TEXTURE_H

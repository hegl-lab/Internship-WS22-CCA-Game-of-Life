#ifndef GAME_OF_LIFE_FRAGMENTONLYSHADER_H
#define GAME_OF_LIFE_FRAGMENTONLYSHADER_H


#include "SimpleShader.h"

class FragmentOnlyShader : public SimpleShader {
public:
    unsigned int VBO, VAO, EBO;
    unsigned int framebuffer;

    explicit FragmentOnlyShader(const char *fragmentPath);

    void init(const std::string &arguments) override;

    // calls the shaders and renders to the current framebuffer
    void render() const;

    // calls the shaders and renders the result to a specific texture
    void render_to_texture(const Texture &target_texture) const;

    // calls the shaders and renders the result to the window
    void render_to_window() const;
};


#endif //GAME_OF_LIFE_FRAGMENTONLYSHADER_H

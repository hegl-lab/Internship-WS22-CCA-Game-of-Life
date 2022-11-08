#ifndef GAME_OF_LIFE_PASSTHROUGHSHADER_H
#define GAME_OF_LIFE_PASSTHROUGHSHADER_H


#include "FragmentOnlyShader.h"

class PassthroughShader : public FragmentOnlyShader {
public:
    PassthroughShader();

    /**
     * Renders a texture to the current frame buffer
     * @param texture texture to render
     */
    void render(const Texture &texture);

    /**
     * Renders a texture into another texture (= one texture gets copied into another texture)
     * @param src_texture texture that to copy
     * @param target_texture texture that top replace
     */
    void render_to_texture(const Texture &src_texture, const Texture &target_texture);

    /**
     * Renders a texture to the window
     * @param texture texture to render
     */
    void render_to_window(const Texture &texture);
};


#endif //GAME_OF_LIFE_PASSTHROUGHSHADER_H

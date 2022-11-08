#include "PassthroughShader.h"

#include <glad/glad.h>
#include "Init.h"

PassthroughShader::PassthroughShader() : FragmentOnlyShader("shaders/passthrough/shader.frag") {}

void PassthroughShader::render(const Texture &texture) {
    glBindVertexArray(VAO);

    bind_uniform("texture1", texture, 1);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void PassthroughShader::render_to_texture(const Texture &src_texture, const Texture &target_texture) {
    glViewport(0, 0, target_texture.width, target_texture.height);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target_texture, 0);

    render(src_texture);
    glViewport(0, 0, CURRENT_WIDTH, CURRENT_HEIGHT);
}

void PassthroughShader::render_to_window(const Texture &texture) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    render(texture);
}
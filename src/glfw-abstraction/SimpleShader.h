#ifndef GAME_OF_LIFE_SIMPLESHADER_H
#define GAME_OF_LIFE_SIMPLESHADER_H

#include <string>
#include "Texture.h"

class SimpleShader {
public:
    unsigned int id;

    SimpleShader();

    SimpleShader(const char *vertexPath, const char *fragmentPath);

    /**
     * Reads and compiles the shaders. MUST be called after gflw has been initialized.
     * @param arguments additional shader code that will be put after the version line in the shaders, useful for constants
     */
    virtual void init(const std::string &arguments);

    /**
     * Call to init(const std::string &) without any arguments.
     */
    void init_without_arguments();

    // use/activate the shader
    void use() const;

    /**
     * Binds a bool to a uniform in the shaders.
     * @param name name of the uniform
     * @param value value to bind
     */
    void bind_uniform(const std::string &name, bool value) const;

    /**
     * Binds an int to a uniform in the shaders.
     * @param name name of the uniform
     * @param value value to bind
     */
    void bind_uniform(const std::string &name, int value) const;

    /**
     * Binds a float value to a uniform in the shaders.
     * @param name name of the uniform
     * @param value value to bind
     */
    void bind_uniform(const std::string &name, float value) const;

    /**
     * Binds a texture to a uniform in the shaders
     * @param name name of the uniform
     * @param texture texture to bind
     * @param unit texture unit to bind to
     */
    void bind_uniform(const char *name, const Texture &texture, int unit) const;

    /**
     * Binds an array to a uniform in the shaders.
     * @param name name of the uniform
     * @param value value to bind
     * @param count number of values in the array
     */
    void bind_uniform(const std::string &name, float *value, int count) const;

    /**
     * Returns the location of the uniform with the given name, can be used to cache
     * locations to save on glGetUniformLocation calls.
     */
    GLint get_location(const std::string &name) const;

    /**
     * Binds a texture to a uniform in the shaders
     * @param location location of the uniform
     * @param texture texture to bind
     * @param unit texture unit to bind to
     */
    void bind_uniform(GLint location, const Texture &texture, int unit) const;

    /**
     * Binds a bool to a uniform in the shaders.
     * @param location location of the uniform
     * @param value value to bind
     */
    void bind_uniform(GLint location, bool value) const;

    /**
     * Binds an int to a uniform in the shaders.
     * @param location location of the uniform
     * @param value value to bind
     */
    void bind_uniform(GLint location, int value) const;

    /**
     * Binds a float value to a uniform in the shaders.
     * @param location location of the uniform
     * @param value value to bind
     */
    void bind_uniform(GLint location, float value) const;

    /**
     * Binds an array to a uniform in the shaders.
     * @param location location of the uniform
     * @param value value to bind
     * @param count number of values in the array
     */
    void bind_uniform(GLint location, float *value, int count) const;
private:
    const char *vertexPath;
    const char *fragmentPath;
};


#endif //GAME_OF_LIFE_SIMPLESHADER_H

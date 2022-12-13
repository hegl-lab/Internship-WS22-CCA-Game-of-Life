#ifndef GAME_OF_LIFE_SIMPLECOMPUTESHADER_H
#define GAME_OF_LIFE_SIMPLECOMPUTESHADER_H

#include <string>
#include "Texture.h"
#include "Buffer.h"

#include <fstream>
#include <iostream>
#include <sstream>

class SimpleComputeShader {
public:
    unsigned int id;

    SimpleComputeShader();

    explicit SimpleComputeShader(const char *path);

    /**
     * Reads and compiles the shader. MUST be called after gflw has been initialized.
     * @param arguments additional shader code that will be put after the version line in the shader, useful for constants
     */
    void init(const std::string &arguments);

    /**
     * Call to init(const std::string &) without any arguments.
     */
    void init_without_arguments();

    // use/activate the shader
    void use() const;

    /**
     * Dispatches the compute shader.
     * The compute shader is run once for every value gl_WorkGroupID in [0, (size_x, size_y, size_z))
     */
    void dispatch(unsigned int size_x, unsigned int size_y, unsigned int size_z) const;

    // wait for the compute shader to finish
    void wait() const;

    /**
     * Binds a texture to a uniform in the compute shader
     * @param name name of the uniform
     * @param texture texture to bind
     * @param unit texture unit to bind to
     * @param access_mode determine if the texture should be read only / write only / read and write.
     * Possible values: (GL_READ_ONLY, GL_WRITE_ONLY, GL_READ_WRITE)
     */
    void bind_uniform(const char *name, const Texture &texture, int unit, int access_mode) const;

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
     * Binds a texture to a uniform in the compute shader
     * @param location location of the uniform
     * @param texture texture to bind
     * @param unit texture unit to bind to
     * @param access_mode determine if the texture should be read only / write only / read and write.
     * Possible values: (GL_READ_ONLY, GL_WRITE_ONLY, GL_READ_WRITE)
     */
    void bind_uniform(GLint location, const Texture &texture, int unit, int access_mode) const;

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

    GLint find_block_index(const std::string &name) const;

    void bind_buffer(GLint location, const Buffer &buffer, int point) const;

    void bind_buffer(const std::string &name, const Buffer &buffer, int point) const;
private:
    const char *path;
};

#endif //GAME_OF_LIFE_SIMPLECOMPUTESHADER_H

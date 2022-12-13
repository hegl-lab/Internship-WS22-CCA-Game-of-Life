#ifndef GAME_OF_LIFE_GLFFT2D_H
#define GAME_OF_LIFE_GLFFT2D_H

#include <Buffer.h>
#include <glfft/glfft.hpp>
#include <glfft/glfft_wisdom.hpp>
#include <glfft/glfft_gl_interface.hpp>

class GLFFT2D {
public:
    GLFFT2D();

    // reads and compiles the shaders for the 2D FFT. MUST be called after gflw has been initialized.
    void init(int width, int height, bool normalize, bool inverse, GLFFT::Type type);

    // calculates the Fourier Transformation of the matrix and saves it to out
    void compute(const Buffer &in, const Buffer &out);
private:
    GLFFT::FFTOptions options;
    GLFFT::FFTWisdom wisdom;
    GLFFT::GLContext context_wisdom;
    GLFFT::GLContext context_fft;
    std::shared_ptr<GLFFT::FFT> fft;
};


#endif //GAME_OF_LIFE_GLFFT2D_H

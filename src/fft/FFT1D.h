#ifndef GAME_OF_LIFE_FFT1D_H
#define GAME_OF_LIFE_FFT1D_H

#include <GLFWAbstraction.h>

class FFT1D {
public:
    explicit FFT1D(bool rows = false, std::string image_format = "rg32f");

    // reads and compiles the shaders for the 1D FFT. MUST be called after gflw has been initialized.
    void init_without_arguments();

    // calculates the Fourier Transformation of the matrix in place
    void compute(const Texture &texture) const;

private:
    bool rows;
    std::string image_format;
    SimpleComputeShader reverse_order;
    SimpleComputeShader fft;
};


#endif //GAME_OF_LIFE_FFT1D_H

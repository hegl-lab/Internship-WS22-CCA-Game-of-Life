#ifndef GAME_OF_LIFE_IFFT2D_H
#define GAME_OF_LIFE_IFFT2D_H

#include "FFT2D.h"

class IFFT2D {
public:
    IFFT2D(const std::string& image_format = "rg32f");

    // reads and compiles the shaders for the 2D IFFT. MUST be called after gflw has been initialized.
    void init_without_arguments();

    // calculates the inverse Fourier Transformation of the matrix in place
    void compute_inverse(const Texture &texture) const;

    FFT2D fft2D;
private:
    SimpleComputeShader conjugate;
    SimpleComputeShader conjugate_scale;
    std::string image_format;
};


#endif //GAME_OF_LIFE_IFFT2D_H

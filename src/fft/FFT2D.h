#ifndef GAME_OF_LIFE_FFT2D_H
#define GAME_OF_LIFE_FFT2D_H

#include "FFT1D.h"

class FFT2D {
public:
    FFT2D(std::string image_format = "rg32f");

    // reads and compiles the shaders for the 2D FFT. MUST be called after gflw has been initialized.
    void init_without_arguments();

    // calculates the Fourier Transformation of the matrix in place
    void compute(const Texture &texture) const;

    FFT1D fft1D_columns;
    FFT1D fft1D_rows;
};

#endif //GAME_OF_LIFE_FFT2D_H

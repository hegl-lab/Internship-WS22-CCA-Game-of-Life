#include <cmath>
#include "FFT2D.h"

FFT2D::FFT2D(std::string image_format) : fft1D_columns(false, image_format), fft1D_rows(true, image_format) {}

void FFT2D::init_without_arguments() {
    fft1D_columns.init_without_arguments();
    fft1D_rows.init_without_arguments();
}

void FFT2D::compute(const Texture &texture) const {
    fft1D_columns.compute(texture);
    fft1D_rows.compute(texture);
}



#include "FFT1D.h"

#include <utility>
#include <cmath>

FFT1D::FFT1D(bool rows, std::string image_format) : rows(rows), image_format(std::move(image_format)),
                                                    fft("shaders/fft/fft1D.comp"),
                                                    reverse_order("shaders/fft/bit-reverse-order.comp") {}

void FFT1D::init_without_arguments() {
    fft.init(generate_arguments_with_default_marker(
                     Argument<std::string>{"IMAGE_FORMAT", image_format}
             ) +
             (rows ? "#define ROWS\n" : "")
    );
    reverse_order.init(generate_arguments_with_default_marker(
                               Argument<std::string>{"IMAGE_FORMAT", image_format}
                       ) +
                       (rows ? "#define ROWS\n" : "")
    );
}

void FFT1D::compute(const Texture &texture) const {
    int x_max = rows ? texture.width : texture.height;
    int y_max = rows ? texture.height : texture.width;

    int number_of_runs = std::log2(x_max);

    reverse_order.use();
    reverse_order.bind_uniform("A", texture, 0, GL_READ_WRITE);
    reverse_order.bind_uniform("n", number_of_runs - 1);
    reverse_order.dispatch(x_max, y_max, 1);
    reverse_order.wait();

    fft.use();
    fft.bind_uniform("A", texture, 0, GL_READ_WRITE);
    GLint location_m = fft.get_location("m");
    GLint location_m2 = fft.get_location("m2");
    GLint location_omega_m_real = fft.get_location("omega_m_real");
    GLint location_omega_m_complex = fft.get_location("omega_m_complex");

    int m2 = 1;
    int m = 2;
    for (int run = 1; run <= number_of_runs; ++run) {
        auto omega_real = float(std::cos(2.0 * M_PI / float(m)));
        auto omega_complex = float(-std::sin(2.0 * M_PI / float(m)));

        fft.bind_uniform(location_m, m);
        fft.bind_uniform(location_m2, m2);
        fft.bind_uniform(location_omega_m_real, omega_real);
        fft.bind_uniform(location_omega_m_complex, omega_complex);

        fft.dispatch(x_max / m, y_max, 1);
        fft.wait();

        m2 *= 2;
        m *= 2;
    }
}

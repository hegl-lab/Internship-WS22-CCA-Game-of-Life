#include "IFFT2D.h"

#include <utility>
#include <cmath>

IFFT2D::IFFT2D(const std::string &image_format) : image_format(image_format), fft2D(image_format),
                                                  conjugate("shaders/fft/conjugate.comp"),
                                                  conjugate_scale("shaders/fft/conjugate-scale.comp") {}

void IFFT2D::compute_inverse(const Texture &texture) const {
    conjugate.use();
    conjugate.bind_uniform("A", texture, 0, GL_READ_WRITE);
    conjugate.dispatch(texture.width, texture.height, 1);
    conjugate.wait();

    fft2D.compute(texture);

    conjugate_scale.use();
    conjugate_scale.bind_uniform("A", texture, 0, GL_READ_WRITE);
    conjugate_scale.bind_uniform("scale", float(texture.width * texture.height));
    conjugate_scale.dispatch(texture.width, texture.height, 1);
    conjugate_scale.wait();

    /*const SimpleComputeShader &fft2d = fft2D.fft2d;
    const SimpleComputeShader &fft2d = fft2D.fft2d;

    scale.use();
    scale.bind_uniform("A", texture, 0, GL_READ_WRITE);
    scale.bind_uniform("scale", float(1.0 / texture.width));

    fft2d.use();
    int location_pass_id = fft2d.get_location("u_PassID");
    fft2d.bind_uniform("u_Dir", 1);
    fft2d.bind_uniform("u_ArgSize", texture.width);
    fft2d.bind_uniform("u_Arg", 0);

    int runs = std::log2(texture.width);
    int cnt = texture.width;
    cnt >>= 1;
    for (int i = 0; i < runs; ++i) {
        int groupCount = cnt >= 32 ? cnt >> 5 : 1;

        fft2d.bind_uniform(location_pass_id, i);
        fft2d.dispatch(groupCount, groupCount, 1);
        fft2d.wait();
    }*/
}

void IFFT2D::init_without_arguments() {
    fft2D.init_without_arguments();

    conjugate.init(
            generate_arguments_with_default_marker(
                    Argument<std::string>{"IMAGE_FORMAT", image_format}
            )
    );
    conjugate_scale.init(
            generate_arguments_with_default_marker(
                    Argument<std::string>{"IMAGE_FORMAT", image_format}
            )
    );
}

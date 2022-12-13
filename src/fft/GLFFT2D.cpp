#include "GLFFT2D.h"

void GLFFT2D::init(int width, int height, bool normalize, bool inverse, GLFFT::Type type) {
    options.type.fp16 = false;
    options.type.input_fp16 = false;
    options.type.output_fp16 = false;
    options.type.normalize = normalize;

    wisdom.set_static_wisdom(GLFFT::FFTWisdom::get_static_wisdom_from_renderer(&context_wisdom));
    wisdom.learn_optimal_options_exhaustive(&context_wisdom, width, height, GLFFT::ComplexToComplex, GLFFT::Image, GLFFT::Image,
                                           options.type);

    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    fft = std::make_shared<GLFFT::FFT>(&context_fft, width, height,
                                       type,
                                       inverse ? GLFFT::Inverse : GLFFT::Forward, GLFFT::SSBO, GLFFT::SSBO,
                                       std::make_shared<GLFFT::ProgramCache>(), options, wisdom);

    //fft -> set_texture_offset_scale(0.5f / width, 0.5f / height, 1.0f / width, 1.0f / height);
}

void GLFFT2D::compute(const Buffer &in, const Buffer &out) {
    GLFFT::GLBuffer adaptor_input(in.id);
    GLFFT::GLBuffer adaptor_output(out.id);

    GLFFT::CommandBuffer *cmd = context_fft.request_command_buffer();

    fft->process(cmd, &adaptor_output, &adaptor_input, &adaptor_input);
    cmd->barrier();
    context_fft.submit_command_buffer(cmd);
    //context_fft.wait_idle();

    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

GLFFT2D::GLFFT2D() {}

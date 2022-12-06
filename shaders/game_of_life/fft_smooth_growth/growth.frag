#version 430 core

// we use the following block to define constants
// this way our IDE knows which constants to expect
// but we can still replace them
#ifndef default_consts
#define m 0.135
#define s 0.015
#define frequency 10.0
#endif // default_consts

#define dt 1.0 / float(frequency)

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D source_state;
uniform sampler2D update;

float growth(float neighbours) {
    return exp(-pow((neighbours - m) / s, 2.0) / 2.0) * 2.0 - 1.0;
}

void main() {
    float new_state = texture(source_state, TexCoord).x + dt * growth(texture(update, TexCoord).x);
    new_state = ((new_state > 1.0) ? 1.0 : ((new_state < 0.0) ? 0.0 : new_state));
    FragColor = vec4(new_state, 0, 0, 1);
}
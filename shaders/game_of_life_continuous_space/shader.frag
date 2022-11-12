#version 430 core

// we use the following block to define constants
// this way our IDE knows which constants to expect
// but we can still replace them
#ifndef default_consts
#define width 60
#define height 60
#define frequency 60.0
#define R 5
#endif // default_consts

#define pixel_x 1.0 / width
#define pixel_y 1.0 / height

#define dt 1.0 / frequency

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform float kernel[(2 * R + 1) * (2 * R + 1)];

float growth(float neighbours) {
    if (neighbours >= 0.12 && neighbours <= 0.15) {
        return 1.0;
    }
    if (neighbours <= 0.12 || neighbours >= 0.15) {
        return -1.0;
    }
    return 0.0;
}

// Returns the new state of the current pixel.
float state(vec2 position) {
    float current = texture(texture1, position).x;

    float neighbours = 0.0;
    for (int x = 0; x < 2 * R + 1; ++x) {
        for (int y = 0; y < 2 * R + 1; ++y) {
            float k = kernel[x * (2 * R + 1) + y];
            if (k != 0.0) {
                neighbours += k * texture(texture1, position + vec2((x - R) * pixel_x, (y - R) * pixel_y)).x;
            }
        }
    }

    float new_state = current + dt * growth(neighbours);
    return (new_state > 1.0) ? 1.0 : ((new_state < 0.0) ? 0.0 : new_state);
}

void main() {
    float new_state = state(TexCoord);
    FragColor = vec4(new_state, new_state, new_state, new_state);
}
#version 430 core

// we use the following block to define constants
// this way our IDE knows which constants to expect
// but we can still replace them
#ifndef default_consts
#define width 60
#define height 60
#endif // default_consts

#define pixel_x 1.0 / width
#define pixel_y 1.0 / height

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;

float growth(float neighbours) {
    if (neighbours >= 2.9 && neighbours <= 3.1) {
        return 1.0;
    }
    if (neighbours <= 1.9 || neighbours >= 3.1) {
        return -1.0;
    }
    return 0.0;
}

// Returns the new state of the current pixel.
float state(vec2 position) {
    uint current = uint(texture(texture1, position).x);
    float kernel[3][3] = {
        {1.0, 1.0, 1.0},
        {1.0, 0.0, 1.0},
        {1.0, 1.0, 1.0}
    };

    float neighbours = 0.0;
    for (int x = 0; x < 3; ++x) {
        for (int y = 0; y < 3; ++y) {
            if (kernel[x][y] != 0.0) {
                neighbours += kernel[x][y] * texture(texture1, position + vec2((x-1) * pixel_x, (y-1) * pixel_y)).x;
            }
        }
    }

    float new_state = current + growth(neighbours);
    return (new_state > 1.0) ? 1.0 : ((new_state < 0.0) ? 0.0 : new_state);
}

void main() {
    float new_state = state(TexCoord);
    FragColor = vec4(new_state, new_state, new_state, new_state);
}
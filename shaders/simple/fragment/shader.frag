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

// Retrieves the state from the input texture at a given position
// and casts it to uint. If the position is out of bounds, 0 is
// returned.
uint state(vec2 position) {
    return uint(texture(texture1, vec2(position)).x);
}

// Checks if the pixel should be alive / dead in the next state according
// to the rules of the game of life.
// The rules can be found here: https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life#Rules
bool check(vec2 position) {
    uint current = uint(texture(texture1, position).x);
    uint neighbours =
    state(position.xy + vec2(pixel_x, 0)) +
    state(position.xy + vec2(0, pixel_y)) +
    state(position.xy + vec2(-pixel_x, 0)) +
    state(position.xy + vec2(0, -pixel_y)) +
    state(position.xy + vec2(pixel_x, pixel_y)) +
    state(position.xy + vec2(pixel_x, -pixel_y)) +
    state(position.xy + vec2(-pixel_x, pixel_y)) +
    state(position.xy + vec2(-pixel_x, -pixel_y));
    return (current == 1) ? (neighbours >= 2 && neighbours <= 3) : (neighbours == 3);
    //return current != 1;
}

void main() {
    if (check(TexCoord)) {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    } else {
        FragColor = vec4(0.0, 0.0, 0.0, 0.0);
    }
}
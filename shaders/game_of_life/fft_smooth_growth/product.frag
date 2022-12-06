#version 430 core

// we use the following block to define constants
// this way our IDE knows which constants to expect
// but we can still replace them
#ifndef default_consts
#define width 100
#define height 100
#endif // default_consts

#define pixel_x 1.0 / width
#define pixel_y 1.0 / height

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D G;
uniform sampler2D K;

void main() {
    vec4 g = texture(G, TexCoord);
    vec4 k = texture(K, TexCoord);

    FragColor = vec4(g.x * k.x - g.y * k.y, g.x * k.y + g.y * k.x, 0, 0);
}
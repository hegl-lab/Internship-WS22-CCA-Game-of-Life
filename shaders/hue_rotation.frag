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

vec4 hsl2rgb(vec3 c) {
    vec3 rgb = clamp( abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0 );
    vec4 final = (c.z + c.y * (rgb-0.5)*(1.0-abs(2.0*c.z-1.0))).rgbr;
    final.a = 1.0;
    return final;
}

void main() {
    FragColor = hsl2rgb(vec3((1.0 - texture(texture1, TexCoord).x) / 1.45, 1.0, 0.5));
}

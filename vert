#version 150

in  vec2 in_Position;
in  vec3 in_Color;

out vec3 ex_Color;
out vec2 textcord;

uniform vec2 bottomLeft;
uniform ivec2 text;

void main(void) {
   gl_Position = vec4(in_Position.x + bottomLeft.x, in_Position.y + bottomLeft.y, 0.0, 1.0);
   textcord = (in_Position  *  vec2(10,-10) + text)/2  ;
   ex_Color = in_Color;
}

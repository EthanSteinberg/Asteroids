#version 150

in  vec2 in_Position;
in  vec2 in_Textcord;

out vec2 mid_Textcord;

void main(void) {
   gl_Position = vec4(in_Position.x, in_Position.y, 0.0, 1.0);
   mid_Textcord = ( in_Textcord)/2  ;
}

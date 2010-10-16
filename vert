#version 150

in  vec2 in_Position;
in  vec2 in_Textcord;
in  vec2 in_PositionScale;

out vec2 mid_Textcord;
out vec2 mid_PositionScale;

uniform vec2 TextScale;

void main(void) 
{
   gl_Position = vec4(in_Position.x, in_Position.y, 0.0, 1.0);
   mid_Textcord = vec2(in_Textcord.x/(TextScale.x),(in_Textcord.y + in_PositionScale.y)/(TextScale.y));
   mid_PositionScale = vec2(in_PositionScale.x ,in_PositionScale.y);
}

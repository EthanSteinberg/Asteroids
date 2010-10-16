#version 150
layout(points) in;
layout(triangle_strip)out;

in vec2 mid_PositionScale[];
in vec2 mid_Textcord[];
out vec2 out_Textcord;

uniform vec2 TextScale;
uniform vec2 Scale;

void main(void)
{
   out_Textcord = mid_Textcord[0];
   gl_Position = gl_in[0].gl_Position;
   EmitVertex();

   out_Textcord = mid_Textcord[0] + vec2(mid_PositionScale[0].x / (TextScale.x),0);
   gl_Position = gl_in[0].gl_Position + vec4(mid_PositionScale[0].x * Scale.x,0,0,0);
   EmitVertex();

   out_Textcord = mid_Textcord[0] + vec2(0,-mid_PositionScale[0].y/ (TextScale.y));
   gl_Position = gl_in[0].gl_Position + vec4(0,mid_PositionScale[0].y * Scale.y,0,0);
   EmitVertex();

   out_Textcord = mid_Textcord[0] + vec2( mid_PositionScale[0].x / (TextScale.x) , -mid_PositionScale[0].y/ (TextScale.y)) ;
   gl_Position = gl_in[0].gl_Position + vec4(mid_PositionScale[0].x * Scale.x,mid_PositionScale[0].y * Scale.y,0,0);
   EmitVertex();
}

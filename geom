#version 150
layout(points) in;
layout(triangle_strip)out;

in vec2 mid_Textcord[];
out vec2 out_Textcord;

uniform ivec2 scale;

void main(void)
{
   out_Textcord = mid_Textcord[0];
   gl_Position = gl_in[0].gl_Position;
   EmitVertex();

   out_Textcord = mid_Textcord[0] + vec2(scale.x * .5,0);
   gl_Position = gl_in[0].gl_Position + vec4(scale.x * 0.1,0,0,0);
   EmitVertex();

   out_Textcord = mid_Textcord[0] + vec2(0,scale.y * -0.5);
   gl_Position = gl_in[0].gl_Position + vec4(0,scale.y * 0.1,0,0);
   EmitVertex();

   out_Textcord = mid_Textcord[0] + vec2(scale.x * 0.5,scale.y * -0.5);
   gl_Position = gl_in[0].gl_Position + vec4(scale.x * 0.1, scale.y * 0.1,0,0);
   EmitVertex();
}

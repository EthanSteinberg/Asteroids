#version 150
layout(points) in;
layout(triangle_strip)out;

in vec2 mid_Textcord[];
out vec2 out_Textcord;

void main(void)
{
   out_Textcord = mid_Textcord[0];
   gl_Position = gl_in[0].gl_Position;
   EmitVertex();

   out_Textcord = mid_Textcord[0] + vec2(0.5,0);
   gl_Position = gl_in[0].gl_Position + vec4(0.2,0,0,0);
   EmitVertex();

   out_Textcord = mid_Textcord[0] + vec2(0,0.5);
   gl_Position = gl_in[0].gl_Position + vec4(0,0.2,0,0);
   EmitVertex();

   out_Textcord = mid_Textcord[0] + vec2(0.5,0.5);
   gl_Position = gl_in[0].gl_Position + vec4(0.2,0.2,0,0);
   EmitVertex();
}

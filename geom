#version 150
layout(triangles) in;
layout(triangle_strip)out;

in vec2  textcordi[];
out vec2 textcord;

void main(void)
{
   textcord = textcordi[0];
   gl_Position = gl_in[0].gl_Position;
   EmitVertex();
   textcord = textcordi[1];
   gl_Position = gl_in[1].gl_Position;
   EmitVertex();
   textcord = textcordi[2];
   gl_Position = gl_in[2].gl_Position;
   EmitVertex();

   EndPrimitive();
}

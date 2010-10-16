#version 150

in  vec2 out_Textcord;

out vec4 Color;

uniform sampler2D Texture;
void main(void) 
{
    Color = texture(Texture,out_Textcord);
    if (Color.a == 0)
      discard;
}

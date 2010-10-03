#version 150

in  vec2 textcord;
in  vec3 ex_Color;

out vec4 Color;

uniform sampler2D MyTexture;

void main(void) 
{
    Color = texture(MyTexture,textcord);
    if (Color.a < 1)
      discard;
}

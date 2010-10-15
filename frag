#version 150

in  vec2 out_Textcord;

out vec4 Color;

uniform sampler2D Texture;
void main(void) 
{
    vec4 tempCol = texture(Texture,out_Textcord);
    if (tempCol.a == 0)
      discard;
    Color = mix(vec4(0,1,1,1),tempCol,tempCol.a);
}

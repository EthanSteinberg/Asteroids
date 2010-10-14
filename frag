#version 150

in  vec2 out_Textcord;

out vec4 Color;

uniform sampler2D Texture;
void main(void) 
{
    float tempCol = texture(Texture,out_Textcord).a;
    Color = mix(vec4(1,0,0,1),vec4(0,0,0,1),tempCol);
    //if (Color == 0)
      //discard;
}

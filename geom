#version 150
layout(points) in;
layout(triangle_strip)out;

in vec2 mid_PositionScale[];
in vec2 mid_Textcord[];
out vec2 out_Textcord;

uniform vec2 TextScale;
uniform vec2 Scale;
uniform float Rotate;

void main(void)
{
   float rad = radians(Rotate);
   
   vec4 lol = gl_in[0].gl_Position;
   float xlol = mid_PositionScale[0].x * Scale.x;
   float ylol = mid_PositionScale[0].y * Scale.y;

   vec4 around = gl_in[0].gl_Position + vec4(mid_PositionScale[0].x * Scale.x/2,mid_PositionScale[0].y * Scale.y/2,0,0) ;

   out_Textcord = mid_Textcord[0];
   gl_Position = vec4(cos(rad) * (lol.x - around.x) - sin(rad)  * (lol.y - around.y) + around.x,sin(rad) * (lol.x - around.x) + cos(rad) * (lol.y - around.y) + around.y,0,1);
   EmitVertex();

   out_Textcord = mid_Textcord[0] + vec2(mid_PositionScale[0].x / (TextScale.x),0);
   lol += vec4(xlol,0,0,0);
   gl_Position = vec4(cos(rad) * (lol.x - around.x) - sin(rad)  * (lol.y - around.y) + around.x,sin(rad) * (lol.x - around.x) + cos(rad) * (lol.y - around.y) + around.y,0,1);
   EmitVertex();

   out_Textcord = mid_Textcord[0] + vec2(0,-mid_PositionScale[0].y/ (TextScale.y));
   lol += vec4(-xlol,ylol,0,0);
   gl_Position = vec4(cos(rad) * (lol.x - around.x) - sin(rad)  * (lol.y - around.y) + around.x,sin(rad) * (lol.x - around.x) + cos(rad) * (lol.y - around.y) + around.y,0,1);
   EmitVertex();

   out_Textcord = mid_Textcord[0] + vec2( mid_PositionScale[0].x / (TextScale.x) , -mid_PositionScale[0].y/ (TextScale.y)) ;
   lol += vec4(xlol,0,0,0);
   gl_Position = vec4(cos(rad) * (lol.x - around.x) - sin(rad)  * (lol.y - around.y) + around.x,sin(rad) * (lol.x - around.x) + cos(rad) * (lol.y - around.y) + around.y,0,1);
   EmitVertex();
}

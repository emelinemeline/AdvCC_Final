#version 150
uniform vec2 mousePos;
uniform float u_time;
out vec4 output_Color;
void main(){

	output_Color = vec4(1.0f,1.0f,1.0f,mousePos.x/gl_FragCoord.x);
}
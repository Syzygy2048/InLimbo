#version 330 core
in vec2 UV;
//in vec3 fragmentColor;
out vec3 color;

uniform sampler2D myTexture
void main()
{
	color = texture(myTexture, UV).rgb;
	
	//color = vec3(1,0,0);
	//color = fragmentColor;
}
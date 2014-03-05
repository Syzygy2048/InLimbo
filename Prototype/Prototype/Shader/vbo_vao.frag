#version 330

in vec3 worldNormal;

out vec4 fragColor;

void main(){
	fragColor = vec4(
	(normalize(worldNormal) + 1) /2,		//only visible --> from -1 to 1 and now from 0 to 1
	1.0);
}
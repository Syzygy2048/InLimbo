#version 330

in vec3 position;
in vec3 normal;

out vec3 worldNormal;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;

void main(void){

	worldNormal = (modelMatrix * vec4(normal,0)).xyz;
	gl_Position = projectionMatrix * modelMatrix * vec4(position, 1);
}
#version 330

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

in vec3 position;
in vec3 normal;
in vec2 uv;

out vec3 worldNormal;
out vec2 fragmentUV;
out vec3 surfacePos;

void main(){
	fragmentUV = uv;
	surfacePos = (viewMatrix * modelMatrix *vec4(position,1.0)).xyz;
	worldNormal = normalize((viewMatrix * modelMatrix * vec4(normal,0.0)).xyz);		//Light comes from viewing direction
	//worldNormal = normalize(modelMatrix * vec4(normal,0)).xyz;							//Light only from ine direction

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position,1.0);
}
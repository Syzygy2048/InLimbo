#version 330

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
//Animation---------------------
//uniform bool animate;
uniform mat4 boneMatrices[60];

in vec4 inBoneWeights;
in vec4 inBoneIndices;
//------------------------------

in vec3 position;
in vec3 normal;
in vec2 uv;
out vec3 worldNormal;
out vec2 fragmentUV;
out vec3 surfacePos;
void main(){
	fragmentUV = uv;
	vec4 boneWeights = inBoneWeights;
	boneWeights = boneWeights / (boneWeights.x + boneWeights.y + boneWeights.z + boneWeights.w);

	mat4 trafo = boneWeights.x * boneMatrices[int(inBoneIndices.x)] + boneWeights.y * boneMatrices[int(inBoneIndices.y)] + boneWeights.z * boneMatrices[int(inBoneIndices.z)] + boneWeights.w * boneMatrices[int(inBoneIndices.w)];
	//mat4 trafo = mat4(1.0f);


	surfacePos = (viewMatrix * modelMatrix * trafo * vec4(position, 1)).xyz;
	worldNormal = normalize((viewMatrix * modelMatrix * trafo * vec4(normal, 0)).xyz);
	gl_Position = projectionMatrix * viewMatrix * modelMatrix *  trafo * vec4(position, 1);
}

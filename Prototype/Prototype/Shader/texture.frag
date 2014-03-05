#version 330

uniform sampler2D colorTexture;

//Materials
uniform vec4 mDiffuse;
uniform vec4 mSpecular;
uniform vec4 mAmbient;
//uniform vec4 mEmission;
uniform float mShininess;

//Light-Source
uniform vec3 sDiffuse;
uniform vec3 sSpecular;
uniform vec3 sAmbient;

//Light Position
uniform vec3 lightpos;

in vec3 worldNormal;
in vec2 fragmentUV;
in vec3 surfacePos;

out vec4 fragColor;

void main(){
	
	//Add source and material
	vec3 ka = mAmbient.xyz*sAmbient;
	vec3 kd = mDiffuse.xyz*sDiffuse;
	vec3 ks = mSpecular.xyz*sSpecular;

	vec3 N = normalize(worldNormal);			//Normal
	vec3 L = normalize(lightpos - surfacePos);	//Light Direction: lightPosition - surfacePositon
	vec3 V = normalize(-surfacePos);			//View direction: from surface to camera
	vec3 H = normalize(L + V);					//Halfway vector
	//vec3 H = (L + V)/length(L + V);

	float dist = length(lightpos - surfacePos);
	float a = 1.0f + dist * 0.0f + pow(dist, 2) * 0.0f;
	vec3 intensity = vec3(2.0f/a, 2.0f/a, 2.0f/a);

	vec3 Ia = ka;
	vec3 Id = intensity * kd * max(dot(L,N), 0.0);
	vec3 Is = intensity * ks * pow(max(dot(N, H), 0.0), mShininess);
	
	vec3 textureColor = texture(colorTexture, fragmentUV).rgb; //Texture-Color

	fragColor = vec4(textureColor * (Ia + Id + Is), 1.0);	
}
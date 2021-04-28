#version 450

in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiTangent;
in vec4 vPosition;

uniform vec3 cameraPosition;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

struct Light
{
	vec3 iA;

	vec3 iD;
	vec3 iS;
	vec3 lightDir;
};

uniform Light light[8];
uniform int lightCount;

uniform vec3 kA;
uniform vec3 kD;
uniform vec3 kS;
uniform float fSpecularPower;

out vec4 fragColor;

void main()
{	
	vec3 n = normalize(vNormal);
	vec3 t = normalize(vTangent);
	vec3 b = normalize(vBiTangent);
	//vec3 l = normalize(lightDir);

	mat3 tbn = mat3(t, b, n);

	vec3 texDiffuse = texture(diffuseTexture, vTexCoord).rgb;
	vec3 texSpecular = texture(specularTexture, vTexCoord).rgb;
	vec3 texNormal = texture(normalTexture, vTexCoord).rgb;

	n = tbn * (texNormal * 2 - 1);

	//float lambertTerm = max(0, min(1, dot(n, -l)));

	vec3 v = normalize(cameraPosition - vPosition.xyz);
	//vec3 r = reflect(l, n);

	//float specularTerm = pow(max(0, dot(r, v)), fSpecularPower);

	//vec3 ambient = iA * kA;
	//vec3 diffuse = iD * kD * texDiffuse * lambertTerm;
	//vec3 specular = iS * kS * texSpecular * specularTerm;

	for (int i = 0; i < lightCount; ++i)
	{
		vec3 l = normalize(light[i].lightDir);

		float lambertTerm = max(0, dot(n, -l));

		vec3 r = reflect(l, n);

		float specularTerm = pow(max(0, dot(r, v)), fSpecularPower);

		vec3 ambient = light[i].iA * kA;
		vec3 diffuse = light[i].iD * kD * texDiffuse * lambertTerm;
		vec3 specular = light[i].iS * kS * texSpecular * specularTerm;

		fragColor += vec4(ambient + diffuse + specular, 1);
	}

	//fragColor = vec4(texDiffuse, 1);
}
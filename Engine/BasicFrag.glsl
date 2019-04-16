#version 430 core

in vec3 o_norm;
in vec3 worldPos;

out vec4 FragColor;

const vec3 LIGHT_DIR = normalize(vec3(0.2, -0.3, 0.1));

void main()
{

	float pct = dot(o_norm, LIGHT_DIR);

	if (pct < 0) pct = 0;

	float finalLight = 0.7 * pct + 0.15;

	vec3 cameraNorm = worldPos;

    FragColor = vec4(finalLight,0,0,1);
	
} 
#version 330

struct Light 
{
	int type;
	vec3 intensity;
	vec3 posdir;
};

uniform vec3 camera_pos;
uniform sampler2D sampler_tex;
uniform vec4 diffuse_colour;

uniform vec3 light_position;
uniform vec3 light_intensity;

uniform Light lights[3];

in vec2 varying_texcoord;
in vec3 varying_normals;
in vec3 varying_position;

out vec4 fragment_colour;

void main(void)
{
	float light_range = 100;
	float material_shininess = 200;
	float spotlight_fov = 90;
	vec3 spotlight_direction = vec3(0, 1, 0);

	vec3 tex_colour = texture(sampler_tex, varying_texcoord).rgb;
	vec3 norm = normalize(varying_normals);
	vec3 pos = varying_position;


	vec3 camera_direction = normalize(camera_pos - pos);

	vec3 ambientLight = vec3(0.1, 0.1, 0.1);
	vec3 ambientColour = tex_colour;
	vec3 ambience = ambientLight * ambientColour;
	vec3 result = ambience;

	vec3 lightColour = vec3(1,1,1);
	vec3 material_specularColour = tex_colour;

	for(int i = 0; i < 3; i++) 
	{ 
		if(lights[i].type == 0)
		{
			vec3 dir = lights[i].posdir; //DIRECTIONAL
			vec3 lightDir = normalize(-dir);
			float intensity = max(0, dot(lightDir, norm));

			vec3 reflectVector = reflect(-lightDir, norm);
			float lightReflect = max(0, dot(camera_direction, reflectVector));

			vec3 specularColour = material_specularColour * pow(lightReflect, material_shininess);

			result += (tex_colour + specularColour) * intensity * lightColour;
		}
		else if (lights[i].type == 1 || lights[i].type == 2)
		{
			float light_distance = distance(lights[i].posdir, pos);
			float attenuation = 1.0 - smoothstep(0, light_range, light_distance);

			vec3 dir = pos - lights[i].posdir; //POINT
			vec3 lightDir = normalize(-dir);
			float intensity = max(0, dot(lightDir, norm));

			vec3 reflectVector = reflect(-lightDir, norm);
			float lightReflect = max(0, dot(camera_direction, reflectVector));

			vec3 specularColour = material_specularColour * pow(lightReflect, material_shininess);

			if(lights[i].type == 2)
			{
				float fc = smoothstep(cos(0.5 * spotlight_fov), 1, dot(lightDir, spotlight_direction));
				attenuation *= fc;
			}

			result += (tex_colour + specularColour) * intensity * lightColour * attenuation;
		}

	}






	//vec3 result = ambience + (tex_colour * intensity * lightColour);

	//tex_colour = tex_colour * intensity + ambience;

	fragment_colour = vec4(result, 1.0);
}
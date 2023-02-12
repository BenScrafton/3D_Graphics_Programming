#version 330

uniform sampler2D sampler_tex;
uniform vec4 diffuse_colour;

in vec2 varying_texcoord;
in vec3 varying_normals;

out vec4 fragment_colour;

void main(void)
{
	vec3 tex_colour = texture(sampler_tex, varying_texcoord).rgb;
	fragment_colour = vec4(tex_colour,1.0);
}
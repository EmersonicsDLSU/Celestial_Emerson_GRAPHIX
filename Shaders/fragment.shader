#version 330 core
uniform vec3 u_color;
out vec4 FragColor;
in vec2 UV;
uniform sampler2D texture_diffuse;

void main()
{
	FragColor = vec4(u_color, 1.0) * texture(texture_diffuse, UV);
	//Simulate Flash Light
	//FragColor = vec4(vec3(1.0 - gl_FlagCoord.z), 1.0) * texture(texture_diffuse, UV);
	//FragColor = vec4(vec3(1.0 - gl_FlagCoord.z), 1.0);
	//FragColor = vec4(u_color, 1.0);
}
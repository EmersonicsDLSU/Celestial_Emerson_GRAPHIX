#version 330 core
uniform vec3 u_color;
out vec4 FragColor;
in vec2 UV;
uniform sampler2D texture_diffuse;
in vec4 vertexColor;
in vec3 o_normal;

void main()
{
	//FragColor = vec4(o_normal, 1.0f);
	//FragColor = vertexColor;
	//FragColor = vec4(u_color, 1.0) * texture(texture_diffuse, UV);
	//Simulate Flash Light
	//FragColor = vec4(vec3(1.0 - gl_FlagCoord.z), 1.0) * texture(texture_diffuse, UV);
	//FragColor = vec4(vec3(1.0 - gl_FlagCoord.z), 1.0);
	FragColor = vertexColor;
	//FragColor = vertexColor * texture(texture_diffuse, UV);
}
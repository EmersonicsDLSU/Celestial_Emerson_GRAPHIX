#version 330 core
uniform vec3 u_color;
out vec3 FragColor;
void main()
{
	FragColor = vec3(u_color);
}
#version 330 core

layout(location = 0) out vec4 vFragColor; // Fragment shader output

void main()
{
	// This simply outputs a constant color
	vFragColor = vec4(1, 1, 1, 1);
}

#version 330 core

layout(location = 0) out vec4 vFragColor; // Fragment shader output

// Input from the vertex shader
smooth in vec4 vSmoothColor; // Interpolated color to fragment shader

void main()
{
	// Set the interp. input color as the shader output to the frame buffer color.
	vFragColor = vSmoothColor;
}

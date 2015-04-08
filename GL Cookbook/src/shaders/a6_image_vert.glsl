#version 330 core

layout(location = 0) in vec2 vVertex; // Object space vertex

// Vertex shader output
smooth out vec2 vUV; // Texture coords for texture lookup in the fragment shader

void main()
{
	// Output the clipspace positions
	gl_Position = vec4(vVertex * 2.0 - 1, 0, 1);

	// Set the input object space vertex position as texture coordinate
	vUV = vVertex;
}

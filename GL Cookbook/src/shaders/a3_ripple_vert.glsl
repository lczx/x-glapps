#version 330 core

// Object space vertex position
layout(location = 0) in vec3 vVertex;

// Uniforms
uniform mat4 MVP; // Combined modelview projection matrix
uniform float time; // Elapsed time

// Shader constants
const float amplitude = 0.125;
const float frequency = 4;
const float PI = 3.14159;

void main()
{
	// Get the Euclidean distance of the current vertex from the center of the mesh
	float distance = length(vVertex);

	// Create a sin function using the distance, multiply freq. and add the elapsed time
	float y = amplitude * sin(-PI * distance * frequency + time);

	// Multiply the MVP matrix with the new position to get the clipspace position
	gl_Position = MVP * vec4(vVertex.x, y, vVertex.z, 1);
}

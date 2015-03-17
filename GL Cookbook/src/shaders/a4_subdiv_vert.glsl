#version 330 core

layout(location = 0) in vec3 vVertex; // Object space vertex position

void main()
{
	// Output object space vertex positions directly,
	// we will elaborate them into the geometry shader.
	gl_Position = vec4(vVertex, 1);
}

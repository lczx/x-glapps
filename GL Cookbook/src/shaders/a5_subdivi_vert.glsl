#version 330 core

layout(location = 0) in vec3 vVertex; // Obj. space vertex position

uniform mat4 M[4]; // Instance modeling matrix

void main()
{
	// Handle the instance modeling matrix and output world space positions
	gl_Position = M[gl_InstanceID] * vec4(vVertex, 1);

	// 'gl_InstanceID is filled with the index of each instance automatically at the
	// time of the 'glDrawElementsInstanced' call. Also note that this built-in
	// attribute is only accessible in the vertex shader.
}

#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 256) out;

// Uniforms
uniform int sub_divisions; // The number of subdivisions
uniform mat4 MVP; // Combined view projection matrix

// This performs the subdivision of the quad
void main()
{
	// Pass-thru
	if (sub_divisions == 0) {
		for (int i = 0; i < gl_in.length(); i++) {
			gl_Position = MVP * gl_in[i].gl_Position;
			EmitVertex();
		}
		return;
	}

	// Get the object space vertex positions
	vec4 v0 = gl_in[0].gl_Position;
	vec4 v1 = gl_in[1].gl_Position;
	vec4 v2 = gl_in[2].gl_Position;

	// Determine the size of each sub-division
	float dx = abs(v0.x - v2.x) / sub_divisions;
	float dz = abs(v0.z - v1.z) / sub_divisions;

	float x = v0.x;
	float z = v0.z;

	// Loop through all sub-divisions and emit vertices
	// after multiplying the object space vertex positions
	// with the combined modelview projection matrix. We
	// move first in x axis, once we reach the edge, we
	// reset x to the initial x value, while incrementing
	// the Z value.
	for (int j = 0; j < sub_divisions * sub_divisions; j++) {
		gl_Position = MVP * vec4(x,      0, z,      1); EmitVertex();
		gl_Position = MVP * vec4(x,      0, z + dz, 1); EmitVertex();
		gl_Position = MVP * vec4(x + dx, 0, z,      1); EmitVertex();
		gl_Position = MVP * vec4(x + dx, 0, z + dz, 1); EmitVertex();
		EndPrimitive();

		x += dx;
		if ((j + 1) % sub_divisions == 0) {
			x = v0.x;
			z += dz;
		}
	}
}

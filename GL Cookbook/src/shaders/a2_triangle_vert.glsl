#version 330 core
// ^^ Version OpenGL 3.3 (since 3.0 format), core profile

// Per-vertex input attributes
//   layout location qualifiers make 'glBindAttribLocation' redundant
//   as the location index specified in the shader overrides any
//   'glBindAttribLocation' call.
//   Book conventions: 0. position; 1. normals; 2. texture coords.
layout(location = 0) in vec3 vVertex; // Object space vertex position
layout(location = 1) in vec3 vColor;  // Per-vertex color

// Output from the vertex shader
//   This attribute is interpolated across shader stages and is so called
//   a 'varying attribute'.
//   By prefixing 'smooth' to the output attribute, we tell the GLSL shader
//   to do smooth perspective-correct interpolation for the attribute to
//   the next stage of the pipeline.
//   The other qualifiers usable are 'flat' and 'noperspective'.
//   When no qualifier is specified, the default interp. qualifier is 'smooth'.
smooth out vec4 vSmoothColor; // Smooth color to fragment shader

// Uniform
uniform mat4 MVP; // Combined modelview projection matrix

void main()
{
	// Assign the per-vertex color to vSmoothColor varying
	vSmoothColor = vec4(vColor, 1);

	// Get the clip space position by multiplying the combined MVP matrix with
	// the object space vertex position
	gl_Position = MVP * vec4(vVertex, 1);
}

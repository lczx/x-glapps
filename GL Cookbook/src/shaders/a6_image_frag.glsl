#version 330 core

layout(location = 0) out vec4 vFragColor; // Fragment shader output

// Input from the vertex shader
smooth in vec2 vUV; // 2D texture coordinates

// Shader uniform
uniform sampler2D textureMap; // The image to display

void main()
{
	// The fragment shader will sample our image texture
	// Sample the 'textureMap' at the given 2D texture coords to obtain the color
	vFragColor = texture(textureMap, vUV);
}

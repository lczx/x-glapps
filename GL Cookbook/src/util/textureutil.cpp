#include "textureutil.h"

#include <iostream>

#include <GL/glew.h>
#include <SOIL.h>

#include "glassert.h"

#pragma comment(lib, "SOIL.lib")

void GenCheckerboardTexture()
{
	// Generate the checker texture
	// This code is be faster if we iterate over 'i', then 'j' (local access).
	GLubyte data[128][128] = { 0 };
	for (int i = 0; i < 128; ++i)
		for (int j = 0; j < 128; ++j)
			data[i][j] = (i <= 64 && j <= 64 || i > 64 && j > 64) ? 255 : 0;

	// Allocate texture object
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 128, 128, 0, GL_RED, GL_UNSIGNED_BYTE, data);
}

void LoadTexture(const std::string &texturePath, bool flip)
{
	// Load the image using SOIL
	int textureWidth = 0, textureHeight = 0, textureChannels = 0;
	// Here, 'SOIL_LOAD_AUTO' keeps all loading settings to default
	GLubyte *pData = SOIL_load_image(texturePath.c_str(),
		&textureWidth, &textureHeight, &textureChannels, SOIL_LOAD_AUTO);

	if (pData == nullptr) { // If it fails, returns NULL
		std::cerr << "Cannot load image: " << texturePath << std::endl;
		assert(pData != nullptr, "Cannot load texture image");
		exit(EXIT_FAILURE);
	}

	if (flip) {
		// Vertically flip the image on Y axis since it is inverted
		for (int j = 0; 2 * j < textureHeight; ++j) {
			int index1 = j * textureWidth * textureChannels;
			int index2 = (textureHeight - 1 - j) * textureWidth * textureChannels;
			for (int i = textureWidth * textureChannels; i > 0; --i) {
				std::swap(pData[index1], pData[index2]);
				++index1, ++index2;
			}
		}
	}

	// Allocate texture
	//   (target, mipmap, internal_fmt, w, h, border, image_fmt, ptr_fmt, data)
	//   internal_fmt by image properties
	// If using texture with alpha (level in photoshop), use GL_RGBA instead
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pData);

	// Free SOIL image data
	SOIL_free_image_data(pData);
}

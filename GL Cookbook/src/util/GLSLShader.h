#pragma once

#include <map>

#include <GL/glew.h>

class GLSLShader
{
public:
	/** Default constructor */
	GLSLShader();

	/** Destructor */
	~GLSLShader();

	/** Load a shader object from a string */
	void loadFromString(GLenum shaderType, const std::string &source);

	/** Load a shader object from a file */
	void loadFromFile(GLenum shaderType, const std::string &fileName);

	/** Creates a program from the loaded shader */
	void createAndLinkProgram();

	/** Bind the shader object on the GPU */
	void use();

	/** Unbind the shader object */
	void unuse();

	/** Store location of the given attribute in the cache */
	void addAttribute(const std::string &attribute);

	/** Store location of the given uniform in the cache */
	void addUniform(const std::string &uniform);

	/** Attribute access indexer */
	GLuint operator[](const std::string &attribute);

	/** Uniform access indexer */
	GLuint operator()(const std::string &uniform);

	/** Free resources allocated by this shader program by deleting it */
	void deleteShaderProgram();

private:
	enum ShaderType { VERTEX_SHADER, FRAGMENT_SHADER, GEOMETRY_SHADER };

	GLuint program_;
	
	int totalShaders_;
	
	GLuint shaders_[3]; // 0: vertex, 1: fragment, 2: geometry

	std::map<std::string, GLuint> attributeLocationList_;

	std::map<std::string, GLuint> uniformLocationList_;
	
};

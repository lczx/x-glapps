#include "GLSLShader.h"

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

// Display utility function
string _GetShaderName(GLenum shaderType)
{
	switch (shaderType) {
	case GL_VERTEX_SHADER: return "vertex";
	case GL_GEOMETRY_SHADER: return "geometry";
	case GL_FRAGMENT_SHADER: return "fragment";
	default: return "other";
	}
}

// Utility to print a program's info log
void _PrintProgramInfoLog(GLuint program, GLenum statusType)
{
	GLint status, infoLogLength;
	glGetProgramiv(program, statusType, &status);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

	string logName = (statusType == GL_LINK_STATUS ? "link" : "verify");
	string logStat = (status == GL_FALSE ? "ERR" : "OK");
	cout << "Shader " << logName << " log " << logStat;

	if (infoLogLength > 1) {
		// We have valid log output
		auto *infoLog = new GLchar[infoLogLength];
		glGetProgramInfoLog(program, infoLogLength, nullptr, infoLog);
		cout << " L" << infoLogLength << ": " << infoLog;
		delete[] infoLog;
	}
	else
		cout << '.' << '\n';
}

GLSLShader::GLSLShader() : totalShaders_(0) /* should work: ', shaders_{0, 0, 0}' */
{
	shaders_[VERTEX_SHADER] = 0;
	shaders_[FRAGMENT_SHADER] = 0;
	shaders_[GEOMETRY_SHADER] = 0;
	attributeLocationList_.clear();
	uniformLocationList_.clear();
}

GLSLShader::~GLSLShader()
{
	// My thoughts:
	//   Maps are local, so they will be destroyed along with this object
	//   and so all of their elements rendering these two calls useless.
	attributeLocationList_.clear();
	uniformLocationList_.clear();
}

void GLSLShader::loadFromString(GLenum shaderType, const string &source)
{
	GLuint shader = glCreateShader(shaderType);

	const char *pTmp = source.c_str();
	glShaderSource(shader, 1, &pTmp, nullptr);

	// Check whether the shader loads fine
	GLint status;
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	//if (status == GL_FALSE) {
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *infoLog = new GLchar[infoLogLength];
		glGetShaderInfoLog(shader, infoLogLength, nullptr, infoLog);
		cerr << "Shader (" << _GetShaderName(shaderType) << ") compile log: " << infoLog;
		delete[] infoLog;
	//}

	shaders_[totalShaders_++] = shader;
}

void GLSLShader::loadFromFile(GLenum shaderType, const string &fileName)
{
	ifstream fp;
	fp.open(fileName, ios_base::in);

	if (fp) {
		string line, buffer;
		while (getline(fp, line)) {
			buffer.append(line);
			buffer.append("\r\n");
		}
		// Copy to source
		loadFromString(shaderType, buffer);
	}
	else
		cerr << "Error loading shader: " << fileName << endl;
}

void GLSLShader::createAndLinkProgram()
{
	program_ = glCreateProgram();

	if (shaders_[VERTEX_SHADER] != 0)
		glAttachShader(program_, shaders_[VERTEX_SHADER]);
	if (shaders_[FRAGMENT_SHADER] != 0)
		glAttachShader(program_, shaders_[FRAGMENT_SHADER]);
	if (shaders_[GEOMETRY_SHADER] != 0)
		glAttachShader(program_, shaders_[GEOMETRY_SHADER]);

	// Link and check whether the program links fine
	glLinkProgram(program_);
	_PrintProgramInfoLog(program_, GL_LINK_STATUS);

	glValidateProgram(program_);
	_PrintProgramInfoLog(program_, GL_VALIDATE_STATUS);

	// Once the program has linked, we can safely delete the objects
	glDeleteShader(shaders_[VERTEX_SHADER]);
	glDeleteShader(shaders_[FRAGMENT_SHADER]);
	glDeleteShader(shaders_[GEOMETRY_SHADER]);
}

void GLSLShader::use()
{
	glUseProgram(program_);
}

void GLSLShader::unuse()
{
	glUseProgram(0);
}

void GLSLShader::addAttribute(const string &attribute)
{
	// Attributes can change during shader execution across
	// different shader stages (ex. per-vertex attributes).
	attributeLocationList_[attribute] = glGetAttribLocation(program_, attribute.c_str());
}

void GLSLShader::addUniform(const string &uniform)
{
	// Uniforms remain constant throughout the shader
	// execution (ex. modelview matrix and texture samplers).
	uniformLocationList_[uniform] = glGetUniformLocation(program_, uniform.c_str());
}

GLuint GLSLShader::operator[](const string &attribute)
{
	return attributeLocationList_[attribute];
}

GLuint GLSLShader::operator()(const string &uniform)
{
	return uniformLocationList_[uniform];
}

void GLSLShader::deleteShaderProgram()
{
	glDeleteProgram(program_);
}

#pragma once

#include <map>
#include <functional>

#include <GL/glew.h>
#include <memory>

/** Represents a Vertex Array Object (VAO) and allows adding data to its VBOs */
class Geometry
{

	/* In OpenGL v3.3 and above, we typically store the geometry information in buffer objects,
	 * which is a linear array of memory managed by the GPU. In order to facilitate the handling
	 * of buffer object(s) during rendering, we use a VERTEX ARRAY OBJECT (VAO). This object stores
	 * references to buffer objects (VBO) that are bound after the VAO is bound. The advantage we get
	 * from using a VAO is that after the VAO is bound, we do not have to bind the buffer object(s) (VBOs). */

	/* Usage hints naming: GL_[modify]_[usage]
	 *   modify. STATIC  - modified once only
	 *           DYNAMIC - modified occasionally
	 *           STREAM  - modified at every use
	 *   usage.  DRAW    - the data will be written but not read
	 *           READ    - the data will be read only
	 *           COPY    - the data will be neither read nor written
	 * With these the GPU and driver can optimize the read/write access to this memory. */

public:
	Geometry()
	{
		/* After the VAO object is generated, we need to remember to bind it to the current OpenGL
		 * context on each class call so that all successive operations affect the attached VAO object. */
		glGenVertexArrays(1, &vaoID_);
	}

	~Geometry()
	{
		for (std::pair<const GLenum, GLuint> &a : bufferObjects_)
			glDeleteBuffers(1, &a.second);
		glDeleteVertexArrays(1, &vaoID_);
	}

	/** Makes a new VBO for this Geometry */
	void generateBufferObject(GLenum bufferType)
	{
		// Generate a new VBO (does this allocate graphics memory? Now?)
		GLuint curVboID;
		glGenBuffers(1, &curVboID);

		// Select this Geometry's VAO and bind the new VBO to it
		glBindVertexArray(vaoID_);
		glBindBuffer(bufferType, curVboID);
		bufferObjects_[bufferType] = curVboID;
	}

	/** Adds static buffer data (will be registered using 'GL_STATIC_DRAW') */
	void addStaticBufferData(GLenum bufferType, const void *data, GLsizeiptr dataElemSz)
	{
		// If we don't have already a VBO with this type, generate it
		if (bufferObjects_.find(bufferType) == bufferObjects_.end()) generateBufferObject(bufferType);

		// Store passed data into the buffer, VBO should be selected automatically by binding its VAO
		glBindVertexArray(vaoID_);
		glBufferData(bufferType, dataElemSz, data, GL_STATIC_DRAW);
	}

	/** Generates buffer data from a lambda generator and adds it using 'addStaticBufferData' */
	template<typename T>
	void genStaticBufferData(GLenum bufferType, size_t arraySz, std::function<void(T * const)> generator)
	{
		auto buffer = std::unique_ptr<T[]>(new T[arraySz]);
		generator(&buffer[0]);
		addStaticBufferData(bufferType, &buffer[0], arraySz * sizeof(T));
	}

private:
	GLuint vaoID_;

	std::map<GLenum, GLuint> bufferObjects_;
	
};

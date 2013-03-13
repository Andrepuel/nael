#ifndef __OPENGL_HPP_0918240129481204810845_
#define __OPENGL_HPP_0918240129481204810845_

#include <nael/matrix.hpp>
#include <memory>
#include <vector>
#include <string>
#include <map>

class SDL_Surface;
namespace nael {

class Shader {
public:
	Shader(unsigned id);
	virtual ~Shader();
	
	void attachTo(unsigned programId);
private:
	unsigned id;
};

class ShaderCode {
public:
	static unsigned FRAGMENT_SHADER;
	static unsigned VERTEX_SHADER;
public:
	ShaderCode(const char*, unsigned type, bool toDelete=true);
	virtual ~ShaderCode();

	std::shared_ptr<Shader> compile();
private:
	const char* source;
	unsigned type;
	bool toDelete;
};

struct ProgramConstructor {
	ProgramConstructor& attach( std::shared_ptr<Shader> shader ) {
		attaches.push_back(shader);
		return *this;
	}
	std::vector< std::shared_ptr<Shader> > attaches;
};

class Program {
public:
	Program( const ProgramConstructor& );
	virtual ~Program();

	void bind();

	unsigned getId() const {
		return id;
	}

	int uniformLocation(const std::string&);
	int attributeLocation(const std::string&);
private:
	unsigned id;
};

class VertexBuffer {
public:
	VertexBuffer(unsigned int size, unsigned elementDimension);
	virtual ~VertexBuffer();

	void data(const float*);

	void bindTo(unsigned programId);

	unsigned size() const {
		return _size;
	}
private:
	bool dataed;
	unsigned id;
	unsigned _size;
	unsigned _dimension;
};

class ProgramContext {
public:
	static unsigned QUADS;
public:
	ProgramContext( std::shared_ptr<Program>, unsigned drawMode );
	virtual ~ProgramContext();

	void setMatrix(const std::string& name, const Matrix3& value);
	void setAttribute(const std::string& name, std::shared_ptr<VertexBuffer> );

	void draw();
private:
	unsigned mode;
	std::shared_ptr<Program> program;
	std::map< std::string,Matrix3 > matrices;
	std::map< std::string,std::shared_ptr<VertexBuffer> > attributes;

};

// cut here -----------------------------------------------------------

class OpenglContext {
public:
	OpenglContext(unsigned width, unsigned height);
	virtual ~OpenglContext();

	/**
	 * Handles events
	 * @return true if the execution must be finished
	 */
	bool poolEvent();
	
	//TODO Create a class for timer functions
	float actualMilliseconds();
	void delay(float milliseconds);

	void beginFrame();
	void endFrame();

	void drawBox(const Matrix3& camera, const Matrix3& boxPos);

private:
	SDL_Surface* surface;
	unsigned w;
	unsigned h;

	std::shared_ptr<Program> defaultProgram;
	std::shared_ptr<VertexBuffer> boxBuffer;
};

};

#endif

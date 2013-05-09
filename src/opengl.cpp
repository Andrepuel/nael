#include <nael/opengl.hpp>
#include <nael/image.hpp>
#include <SDL.h>
#include <GL/glew.h>
#include <stdexcept>
#include <cassert>
#include <sstream>

namespace nael {

namespace {
	const char* errorname(unsigned a) {
		switch(a) {
		case GL_NO_ERROR:                      return "GL_NO_ERROR";
		case GL_INVALID_ENUM:                  return "GL_INVALID_ENUM";
		case GL_INVALID_VALUE:                 return "GL_INVALID_VALUE";
		case GL_INVALID_OPERATION:             return "GL_INVALID_OPERATION";
		case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
		case GL_OUT_OF_MEMORY:                 return "GL_OUT_OF_MEMORY";
		}
		return "";
	}

	void checkGlError_impl(unsigned int line, const char* file) {
		unsigned error = glGetError();
		if( error != GL_NO_ERROR ) {
			std::stringstream ss;
			ss << "Opengl error " << errorname(error) << " on " << file << ":" << line;
			throw std::runtime_error(ss.str());
		}
	};
	#define checkGlError() checkGlError_impl(__LINE__,__FILE__);

	SDL_Surface* initGl(unsigned width, unsigned height) {
		if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
			throw std::runtime_error("Could not initialize SDL Video");
		}

		int video_flags;
		video_flags = SDL_OPENGL;
		video_flags |= SDL_GL_DOUBLEBUFFER;
		video_flags |= SDL_HWSURFACE;
		video_flags |= SDL_HWACCEL;
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
		SDL_Surface* surface = SDL_SetVideoMode( width, height, 24, video_flags );
		if( surface == NULL )
			throw std::runtime_error("Could not create window");

		return surface;
	}
}

// cut here ------------------------------------------------------------------

Shader::Shader(unsigned id)
:id(id)
{
}
Shader::~Shader() {
	glDeleteShader(id);
}

void Shader::attachTo(unsigned program) {
	glAttachShader(program,id);
	checkGlError();
}

// --------------------

unsigned ShaderCode::VERTEX_SHADER = GL_VERTEX_SHADER;
unsigned ShaderCode::FRAGMENT_SHADER = GL_FRAGMENT_SHADER;

ShaderCode::ShaderCode(const char* code, unsigned type, bool toDelete)
:source(code),type(type),toDelete(toDelete)
{
}
ShaderCode::~ShaderCode() {
	if( toDelete )
		delete[] source;
}

std::shared_ptr<Shader> ShaderCode::compile() {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader,1,&source,NULL);

	glCompileShader(shader);
	GLint compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if( compileStatus == GL_FALSE ) {
		int msgLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &msgLength);

		char* msg = new char[msgLength];
		glGetShaderInfoLog(shader, msgLength, &msgLength, msg);

		std::runtime_error except(std::string("Error on compiling shader:\n")+msg);
		delete[] msg;
		throw except;
	}
	checkGlError();

	return std::make_shared<Shader>(shader);
};

// --------------------

Program::Program(const ProgramConstructor& constructor) {
	id = glCreateProgram();
	checkGlError();
	for( auto each = constructor.attaches.begin(); each != constructor.attaches.end(); ++each ) {
		(*each)->attachTo(id);
	};

	glLinkProgram(id);
	GLint linkStatus;
	glGetProgramiv(id, GL_LINK_STATUS, &linkStatus);
	if( linkStatus == GL_FALSE ) {
		int msgLength;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &msgLength);

		char* msg = new char[msgLength];
		glGetProgramInfoLog(id, msgLength, &msgLength, msg);

		std::runtime_error except(std::string("Error on linking shader:\n")+msg);
		delete[] msg;
		throw except;
	}
	checkGlError();
};

Program::~Program() {
	glDeleteProgram(id);
}

void Program::bind() {
	glUseProgram(id);
}

int Program::uniformLocation(const std::string& name) {
	int loc = glGetUniformLocation(id,name.c_str());
	checkGlError();
	return loc;
}

int Program::attributeLocation(const std::string& name) {
	int loc = glGetAttribLocation(id,name.c_str());
	checkGlError();
	return loc;
}

// --------------------

VertexBuffer::VertexBuffer(unsigned size, unsigned dimension)
:_size(size),_dimension(dimension)
{
	glGenBuffers(1,&id);
	dataed = false;
	checkGlError();
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1,&id);
}

void VertexBuffer::data(const float* datas) {
	dataed = true;
	glBindBuffer(GL_ARRAY_BUFFER,id);
	glBufferData(GL_ARRAY_BUFFER,_size*_dimension*sizeof(float),datas,GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	checkGlError();
}

void VertexBuffer::bindTo(unsigned location) {
	assert(dataed);
	checkGlError();
	glBindBuffer(GL_ARRAY_BUFFER,id);
	glVertexAttribPointer(location,_dimension,GL_FLOAT,false,0,0);
	checkGlError();
}

// --------------------

Texture::Texture(const Image& image) {
	glGenTextures(1,&id);
	checkGlError();

	GLint format = GL_RGBA;
	glBindTexture(GL_TEXTURE_2D,id);
	glPixelStorei(GL_PACK_ALIGNMENT,1);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexImage2D(GL_TEXTURE_2D,0,format,image.w(),image.h(),0,format,GL_UNSIGNED_BYTE,image.bytes());
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//	glGenerateMipmap(GL_TEXTURE_2D);
	checkGlError();	
}

Texture::~Texture() {
	glDeleteTextures(1,&id);
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D,id);
	checkGlError();
}

// --------------------

unsigned ProgramContext::QUADS = GL_QUADS;
ProgramContext::ProgramContext(std::shared_ptr<Program> program, unsigned mode)
:mode(mode),program(program)
{
}

ProgramContext::~ProgramContext() {
}

void ProgramContext::setMatrix(const std::string& name, const Matrix3& mat) {
	assert( matrices.count(name) == 0 );
	matrices.insert( std::make_pair(name,mat) );
}

void ProgramContext::setBool(const std::string& name, bool value ) {
	assert( bools.count(name) == 0 );
	bools.insert( std::make_pair(name,value) );
}

void ProgramContext::setTexture(const std::string& name, const std::shared_ptr<Texture>& texture) {
	assert( textures.count(name) == 0 );
	textures.insert( std::make_pair(name,texture) );
}

void ProgramContext::setAttribute(const std::string& name, std::shared_ptr<VertexBuffer> vbo) {
	assert( attributes.count(name) == 0 );
	attributes.insert( std::make_pair(name,vbo) );
}

void ProgramContext::draw() {
	program->bind();

	checkGlError();
	assert( !attributes.empty() );
	for( auto each = matrices.begin(); each != matrices.end(); ++each ) {
		int location = program->uniformLocation(each->first);
		if( location < 0 ) continue;
		glUniformMatrix3fv( location, 1, false, each->second.raw() );
		checkGlError();
	}
	//TODO Remove uniform repetition
	for( auto each = bools.begin(); each != bools.end(); ++each ) {
		int location = program->uniformLocation(each->first);
		if( location < 0 ) continue;
		glUniform1i(location,each->second);
		checkGlError();
	}
	unsigned textureSlot = 0;
	for( auto each = textures.begin(); each != textures.end(); ++each ) {
		int location = program->uniformLocation(each->first);
		if( location < 0 ) continue;
		glActiveTexture(GL_TEXTURE0 + textureSlot);
		each->second->bind();
		glUniform1i(location,textureSlot);
		++textureSlot;
	}
	checkGlError();
	std::vector<int> attribs;
	for( auto each = attributes.begin(); each != attributes.end(); ++each ) {
		int location = program->attributeLocation(each->first);
		if( location < 0 ) continue;
		glEnableVertexAttribArray(location);
		attribs.push_back(location);
		each->second->bindTo(location);
	}
	checkGlError();
	glDrawArrays(mode,0,attributes.begin()->second->size());
	checkGlError();
	
	for( auto each = attribs.begin(); each != attribs.end(); ++each ) {
		glDisableVertexAttribArray(*each);
	}

	checkGlError();
};

// cut here ------------------------------------------------------------------

OpenglContext::OpenglContext(unsigned width, unsigned height)
: w(width), h(height)
{
	surface = initGl(width,height);
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::stringstream ss;
		ss << "Error loading GLEW: " << glewGetErrorString(err);
		throw std::runtime_error(ss.str());
	}

	glViewport(0,0,width,height);
	glClearColor(1,1,1,1);
	glClearDepth(1);
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );

	checkGlError();
}

OpenglContext::~OpenglContext() {
	SDL_Quit();
}

bool OpenglContext::poolEvent() {
	SDL_Event event;
	while( SDL_PollEvent(&event) ) {
		switch( event.type ) {
		case SDL_QUIT:
			return true;
		break;
		}
	}
	return false;
}

float OpenglContext::actualMilliseconds() {
	return SDL_GetTicks();
}
void OpenglContext::delay(float milliseconds) {
	SDL_Delay(milliseconds);
}

void OpenglContext::beginFrame() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void OpenglContext::endFrame() {
	SDL_GL_SwapBuffers();
}

namespace {
std::shared_ptr<Program> initDefaultProgram();
std::shared_ptr<VertexBuffer> initBoxBuffer();
std::shared_ptr<VertexBuffer> initUvBuffer();
};

void OpenglContext::drawBox(const Matrix3& camera, const Matrix3& boxPos) {
	_initDefaultShader();

	ProgramContext context(defaultProgram,ProgramContext::QUADS);
	context.setMatrix("camera",camera);
	context.setMatrix("model",boxPos);
	context.setBool("has_texture",false);
	context.setAttribute("vertex",boxBuffer);
	context.draw();
}

void OpenglContext::drawTexture(const Matrix3& camera, const Matrix3& boxPos, const std::shared_ptr<Texture>& texture) {
	_initDefaultShader();

	ProgramContext context(defaultProgram,ProgramContext::QUADS);
	context.setMatrix("camera",camera);
	context.setMatrix("model",boxPos);
	context.setBool("has_texture",true);
	context.setTexture("texture",texture);
	context.setAttribute("vertex",boxBuffer);
	context.setAttribute("uv",uvBuffer);
	context.draw();
};

void OpenglContext::_initDefaultShader() {
	if( defaultProgram.get() == NULL )
		defaultProgram = initDefaultProgram();
	if( boxBuffer.get() == NULL )
		boxBuffer = initBoxBuffer();
	if( uvBuffer.get() == NULL )
		uvBuffer = initUvBuffer();
}

namespace {
const char* default_vert = 	"uniform mat3 camera;\n"
				"uniform mat3 model;\n"
				"attribute vec2 vertex;\n"
				"attribute vec2 uv;\n"
				"varying vec2 texture_pos;\n"
				"void main(void) {\n"
				"	vec3 vertexLine = camera*model*vec3(vertex,1);\n"
				"	gl_Position = vec4(vertexLine.x,vertexLine.y,0,vertexLine.z);\n"
				"	texture_pos = uv;\n"
				"}\n";

const char* default_frag =	"uniform bool has_texture;\n"
				"uniform sampler2D texture;\n"
				"varying vec2 texture_pos;\n"
				"void main(void) {\n"
				"	if( has_texture ) {\n"
				"		gl_FragColor = texture2D(texture,texture_pos);\n"
				"	} else {\n"
				"		gl_FragColor = vec4(0,0,0,1);\n"
				"	}"
				"}\n";

std::shared_ptr<Program> initDefaultProgram() {
	std::shared_ptr<ShaderCode> vert_code( new ShaderCode(default_vert,ShaderCode::VERTEX_SHADER,false) );
	std::shared_ptr<ShaderCode> frag_code( new ShaderCode(default_frag,ShaderCode::FRAGMENT_SHADER,false) );

	return std::make_shared<Program>( ProgramConstructor().attach(vert_code->compile()).attach(frag_code->compile()) );
}

std::shared_ptr<VertexBuffer> initBoxBuffer() {
	std::shared_ptr<VertexBuffer> box_vbo(new VertexBuffer(4,2));
	static const float data[4*2] = {-1,-1, 1,-1, 1,1, -1,1};
	box_vbo->data(data);
	return box_vbo;
}

std::shared_ptr<VertexBuffer> initUvBuffer() {
	std::shared_ptr<VertexBuffer> uv_vbo(new VertexBuffer(4,2));
	static const float data[4*2] = {0,0, 1,0, 1,1, 0,1};
	uv_vbo->data(data);
	return uv_vbo;
}
}//namespace

};

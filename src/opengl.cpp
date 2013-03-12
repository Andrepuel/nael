#include <nael/opengl.hpp>
#include <SDL.h>
#include <GL/glew.h>
#include <stdexcept>

#include <sstream>

namespace nael {

namespace {
	void checkGlError_impl(unsigned int line, const char* file) {
		if( glGetError() != GL_NO_ERROR ) {
			std::stringstream ss;
			ss << "Opengl error on " << file << ":" << line;
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
	//TODO Free SDL stuff
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

};

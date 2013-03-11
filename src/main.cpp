#include <iostream>
#include <stdexcept>

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <sstream>

#define WIDTH 800
#define HEIGHT 600

void checkGlError_impl(unsigned int line, const char* file) {
	if( glGetError() != GL_NO_ERROR ) {
		std::stringstream ss;
		ss << "OpenGL error on " << file << ":" << line;
		throw std::runtime_error(ss.str());
	}
};
#define checkGlError() checkGlError_impl(__LINE__,__FILE__);

SDL_Surface* setupContext() {
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		throw std::runtime_error("Could not initialize SDL Video");
	}

	int video_flags;
	video_flags = SDL_OPENGL;
	video_flags |= SDL_GL_DOUBLEBUFFER;
	video_flags |= SDL_HWSURFACE;
	video_flags |= SDL_HWACCEL;
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_Surface* surface = SDL_SetVideoMode( WIDTH, HEIGHT, 24, video_flags );
	if( surface == NULL )
		throw std::runtime_error("Could not create window");

	return surface;
}

void setupOpenGL() {
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::stringstream ss;
		ss << "Error loading GLEW: " << glewGetErrorString(err);
		throw std::runtime_error(ss.str());
	}

	glViewport(0,0,WIDTH,HEIGHT);
	glClearColor(1,1,1,1);
	glClearDepth(1);
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );

	checkGlError();
}

int main() {
	SDL_Surface* surface = setupContext();
	setupOpenGL();

	while( true ) {
		SDL_Event event;
		while( SDL_PollEvent(&event) ) {
			switch( event.type ) {
			case SDL_QUIT:
				return 0;
			break;
			}
		}	

		float actualMs = SDL_GetTicks();
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		//DRAW

		SDL_GL_SwapBuffers();
		SDL_Delay(1000/60);
	}

	return 0;
};

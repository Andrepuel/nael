#include <iostream>

#include <nael/matrix.hpp>
#include <nael/opengl.hpp>

#define WIDTH 800
#define HEIGHT 600

int main() {
	nael::OpenglContext context(WIDTH,HEIGHT);

	while( true ) {
		if( context.poolEvent() )
			break;

		float actualMs = context.actualMilliseconds();
		context.beginFrame();
		//DRAW
		context.endFrame();
		context.delay(1000/60.0);
	}

	return 0;
};

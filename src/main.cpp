#include <nael/matrix.hpp>
#include <nael/opengl.hpp>

#define WIDTH 800
#define HEIGHT 600

int main() {
	nael::OpenglContext context(WIDTH,HEIGHT);

	using nael::Matrix3;
	Matrix3 camera = Matrix3::scale(0.01,0.01); // The area from (-100,-100) to (100,100) is visible

	while( true ) {
		if( context.poolEvent() )
			break;

		float actualMs = context.actualMilliseconds();
		context.beginFrame();

		using nael::Degree;
		Matrix3 boxPos = Matrix3::translate(-20,40)*Matrix3::rotate(Degree(30))*Matrix3::scale(20,20);
		context.drawBox(camera,boxPos);

		context.endFrame();
		context.delay(1000/60.0);
	}

	return 0;
};

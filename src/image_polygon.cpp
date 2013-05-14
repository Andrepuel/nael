#include <nael/matrix.hpp>
#include <nael/opengl.hpp>
#include <nael/image.hpp>
#include <iostream>

#define PADDING (100.0)

int main(int argc, char** argv) {
	using namespace nael;

	if( argc < 2 ) {
		std::cerr << argv[0] << " is a tool to draw a polygon over an image and extract it's vertices." << std::endl;
		std::cerr << "Usage: " << argv[0] << " image_path" << std::endl;

		return 1;
	}
	Image image(argv[1]);
	OpenglContext context(image.w()+PADDING*2,image.h()+PADDING*2);

	std::shared_ptr<Texture> texture(new Texture(image));

	while( true ) {
		if( context.poolEvent() )
			break;

		context.beginFrame();

		context.drawTexture(Matrix3::identity(), Matrix3::scale(image.w()/(image.w()+PADDING*2), image.h()/(image.h()+PADDING*2)),texture);

		context.endFrame();
		context.delay(1000/60.0);
	}

	return 0;
};

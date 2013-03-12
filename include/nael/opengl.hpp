#ifndef __OPENGL_HPP_0918240129481204810845_
#define __OPENGL_HPP_0918240129481204810845_

class SDL_Surface;
namespace nael {

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
private:
	SDL_Surface* surface;
	unsigned w;
	unsigned h;
};

};

#endif

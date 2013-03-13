#ifndef __ANGLE_HPP_019480851785197517890_
#define __ANGLE_HPP_019480851785197517890_

//TODO math nael header
#include <cmath>
#ifndef M_PI
#define M_PI 3.1415926536
#endif

namespace nael {

struct Degree {
	Degree(float value) : value(value)
	{
	}
	float value;
};

struct Radian {
	Radian(float value) : value(value)
	{
	}
	float value;
};

class Angle {
public:
	Angle(const Degree& degree) : radian( (degree.value/180.0)*M_PI ) {}
	Angle(const Radian& radian) : radian( radian.value ) {}

	float getRadian() const {
		return radian;
	}

	float getDegree() const {
		return radian*(180.0/M_PI);
	}
private:
	float radian;
};

};

#endif

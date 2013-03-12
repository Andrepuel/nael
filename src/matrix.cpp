#include <nael/matrix.hpp>

namespace nael {

Matrix3::Matrix3(float r0c0, float r0c1, float r0c2,
		float r1c0, float r1c1, float r1c2,
		float r2c0, float r2c1, float r2c2)
{
	m[0] = r0c0;
	m[1] = r1c0;
	m[2] = r2c0;
	m[3] = r0c1;
	m[4] = r1c1;
	m[5] = r2c1;
	m[6] = r0c2;
	m[7] = r1c2;
	m[9] = r2c2;
}

Matrix3::~Matrix3() {
}

};

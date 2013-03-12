#ifndef __MATRIX_HPP_41209481041890412_
#define __MATRIX_HPP_41209481041890412_

namespace nael {

class Matrix3 {
public:
	static const Matrix3& getIdentity();
public:
	Matrix3() {
	}
	Matrix3(float r0c0, float r0c1, float r0c2,
		float r1c0, float r1c1, float r1c2,
		float r2c0, float r2c1, float r2c2);

	virtual ~Matrix3();

	const float* raw() const {
		return m;
	};
private:
	float m[9];
};

};

#endif

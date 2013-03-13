#include <nael/matrix.hpp>
#include <cmath>

namespace nael {

Matrix3 Matrix3::scale(float sx, float sy) {
	return Matrix3(sx,0,0,
			0,sy,0,
			0,0,1);
};
Matrix3 Matrix3::rotate(const Angle& angle) {
	float cossine = std::cos(angle.getRadian());
	float sine = std::sin(angle.getRadian());

	return Matrix3( cossine,-sine,0,
			sine, cossine,0,
			0   , 0   ,1);
}
Matrix3 Matrix3::translate(float tx, float ty) {
	return Matrix3( 1,0,tx,
			0,1,ty,
			0,0,1);
}


// cut here -------------------------------------------------

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

#define __elem(COL,ROW,COL_SIZE) (ROW*COL_SIZE+COL)
#define _elem(COL,ROW) __v(COL,ROW,3)

#define _multiply(left,right,dest,left_lines,left_cols_right_lines,right_cols) \
	for( unsigned int eachLine = 0; eachLine < left_lines; ++eachLine ) { \
		for( unsigned int eachCol = 0; eachCol < right_cols; ++eachCol ) {\
			dest[ __elem(eachLine,eachCol,right_cols) ] = 0.0; \
			for( unsigned int eachElement = 0; eachElement < left_cols_right_lines; ++eachElement ) { \
				dest[ __elem(eachLine,eachCol,right_cols) ] += \
					left[ __elem(eachLine,eachElement,left_cols_right_lines) ] * \
					right[ __elem(eachElement,eachCol,right_cols) ]; \
			} \
		} \
	}

Matrix3 Matrix3::operator*(const Matrix3& that) const {
	Matrix3 result;
	_multiply(raw(),that.raw(),result.m,3,3,3);
	return result;
}

};

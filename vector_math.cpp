#include "vector_math.h"
#include "utils.h"
using namespace Utils;

namespace vector_math {
	//VECTOR MATH
	vec3d Vector_MultiplyMatrix(vec3d v, mat4x4 m) {
		float x = v.x * m.m[0][0] + v.y * m.m[0][1] + v.z * m.m[0][2] + v.w * m.m[0][3];
		float y = v.x * m.m[1][0] + v.y * m.m[1][1] + v.z * m.m[1][2] + v.w * m.m[1][3];
		float z = v.x * m.m[2][0] + v.y * m.m[2][1] + v.z * m.m[2][2] + v.w * m.m[2][3];
		float w = v.x * m.m[3][0] + v.y * m.m[3][1] + v.z * m.m[3][2] + v.w * m.m[3][3];
		return { x,y,z,w };
	}
	vec3d Vector_CrossProduct(vec3d u, vec3d v) {
		return { u.y*v.z - u.z*v.y,
				u.z*v.x - u.x*v.z,
				u.x*v.y - u.y*v.x };
	}
	vec3d Vector_Add(vec3d a, vec3d b) {
		return { a.x + b.x,a.y + b.y,a.z + b.z };
	}
	vec3d Vector_Sub(vec3d a, vec3d b) {
		return { a.x - b.x,a.y - b.y,a.z - b.z };
	}
	vec3d Vector_Mul(vec3d a, float sc) {
		return { a.x*sc,a.y*sc,a.z*sc };
	}
	vec3d Vector_Div(vec3d a, float sc) {
		return { a.x / sc,a.y / sc,a.z / sc };
	}
	vec3d Vector_Normalize(vec3d v) {
		float l = Vector_Magnitude(v);
		return { v.x / l,v.y / l,v.z / l };
	}
	float Vector_DotProduct(vec3d a, vec3d b) {
		return a.x*b.x + a.y*b.y + a.z*b.z;
	}
	float Vector_Magnitude(vec3d v) {
		return sqrtf(Vector_DotProduct(v, v));
	}
	//////////////////////////////////////////MATRIX/////////////////////////////////////////
	//Generate special kinds of matrices...
	mat4x4 GenerateTranslationMatrix(vec3d v) {
		return { { {1,0,0,v.x},
				   {0,1,0,v.y},
				   {0,0,1,v.z},
				   {0,0,0,  1} } };
	}
	mat4x4 GenerateRotationMatrixX(float a) {
		return { {{1,0,      0,       0},
				  {0,cosf(a),-sinf(a),0},
				  {0,sinf(a), cos(a), 0},
				  {0,0      , 0     , 1}} };
	}
	mat4x4 GenerateRotationMatrixY(float a) {
		return { {{cosf(a),0,-sinf(a),0},
				  {0      ,1, 0      ,0},
				  {sinf(a),0, cosf(a),0},
				  {0      ,0, 0      ,1}} };
	}
	mat4x4 GenerateRotationMatrixZ(float a) {
		return { {{cosf(a),-sinf(a),0,0},
				  {sinf(a), cosf(a),0,0},
				  {0      , 0      ,1,0},
				  {0      , 0      ,0,1}} };
	}
	mat4x4 GenerateIdentityMatrix() {
		return {
			{
				{1,0,0,0},
				{0,1,0,0},
				{0,0,1,0},
				{0,0,0,1}
			}
		};
	}
	//Functions
	mat4x4 Matrix_MultiplyMatrix(mat4x4 a, mat4x4 b) {
		float a0, b0, c0, d0, a1, b1, c1, d1, a2, b2, c2, d2, a3, b3, c3, d3;
		a0 = a.m[0][0] * b.m[0][0] + a.m[0][1] * b.m[1][0] + a.m[0][2] * b.m[2][0] + a.m[0][3] * b.m[3][0];
		b0 = a.m[0][0] * b.m[0][1] + a.m[0][1] * b.m[1][1] + a.m[0][2] * b.m[2][1] + a.m[0][3] * b.m[3][1];
		c0 = a.m[0][0] * b.m[0][2] + a.m[0][1] * b.m[1][2] + a.m[0][2] * b.m[2][2] + a.m[0][3] * b.m[3][2];
		d0 = a.m[0][0] * b.m[0][3] + a.m[0][1] * b.m[1][3] + a.m[0][2] * b.m[2][3] + a.m[0][3] * b.m[3][3];

		a1 = a.m[1][0] * b.m[0][0] + a.m[1][1] * b.m[1][0] + a.m[1][2] * b.m[2][0] + a.m[1][3] * b.m[3][0];
		b1 = a.m[1][0] * b.m[0][1] + a.m[1][1] * b.m[1][1] + a.m[1][2] * b.m[2][1] + a.m[1][3] * b.m[3][1];
		c1 = a.m[1][0] * b.m[0][2] + a.m[1][1] * b.m[1][2] + a.m[1][2] * b.m[2][2] + a.m[1][3] * b.m[3][2];
		d1 = a.m[1][0] * b.m[0][3] + a.m[1][1] * b.m[1][3] + a.m[1][2] * b.m[2][3] + a.m[1][3] * b.m[3][3];

		a2 = a.m[2][0] * b.m[0][0] + a.m[2][1] * b.m[1][0] + a.m[2][2] * b.m[2][0] + a.m[2][3] * b.m[3][0];
		b2 = a.m[2][0] * b.m[0][1] + a.m[2][1] * b.m[1][1] + a.m[2][2] * b.m[2][1] + a.m[2][3] * b.m[3][1];
		c2 = a.m[2][0] * b.m[0][2] + a.m[2][1] * b.m[1][2] + a.m[2][2] * b.m[2][2] + a.m[2][3] * b.m[3][2];
		d2 = a.m[2][0] * b.m[0][3] + a.m[2][1] * b.m[1][3] + a.m[2][2] * b.m[2][3] + a.m[2][3] * b.m[3][3];

		a3 = a.m[3][0] * b.m[0][0] + a.m[3][1] * b.m[1][0] + a.m[3][2] * b.m[2][0] + a.m[3][3] * b.m[3][0];
		b3 = a.m[3][0] * b.m[0][1] + a.m[3][1] * b.m[1][1] + a.m[3][2] * b.m[2][1] + a.m[3][3] * b.m[3][1];
		c3 = a.m[3][0] * b.m[0][2] + a.m[3][1] * b.m[1][2] + a.m[3][2] * b.m[2][2] + a.m[3][3] * b.m[3][2];
		d3 = a.m[3][0] * b.m[0][3] + a.m[3][1] * b.m[1][3] + a.m[3][2] * b.m[2][3] + a.m[3][3] * b.m[3][3];

		return {
			{
				{a0,b0,c0,d0},
				{a1,b1,c1,d1},
				{a2,b2,c2,d2},
				{a3,b3,c3,d3},
			}
		};
	}
}
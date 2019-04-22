#ifndef vector_math_h
#define vector_math_h
#include "utils.h"

namespace vector_math {
	//Vector math
	Utils::vec3d Vector_MultiplyMatrix(Utils::vec3d v, Utils::mat4x4 m);
	Utils::vec3d Vector_CrossProduct(Utils::vec3d u, Utils::vec3d v);
	Utils::vec3d Vector_Add(Utils::vec3d a, Utils::vec3d b);
	Utils::vec3d Vector_Sub(Utils::vec3d a, Utils::vec3d b);
	Utils::vec3d Vector_Mul(Utils::vec3d a, float sc);
	Utils::vec3d Vector_Div(Utils::vec3d a, float sc);
	Utils::vec3d Vector_Normalize(Utils::vec3d v);
	Utils::vec3d Vector_RotateAroundVector(Utils::vec3d v, Utils::vec3d k, float a);
	float Vector_DotProduct(Utils::vec3d a, Utils::vec3d b);
	float Vector_Magnitude(Utils::vec3d v);
	/////////////////MATRICES//////////////////
	//Generates
	Utils::mat4x4 GenerateTranslationMatrix(Utils::vec3d v);
	Utils::mat4x4 GenerateRotationMatrixX(float a);
	Utils::mat4x4 GenerateRotationMatrixY(float a);
	Utils::mat4x4 GenerateRotationMatrixZ(float a);
	Utils::mat4x4 GenerateIdentityMatrix();
	//Functions
	Utils::mat4x4 Matrix_MultiplyMatrix(Utils::mat4x4 a, Utils::mat4x4 b);
}

#endif
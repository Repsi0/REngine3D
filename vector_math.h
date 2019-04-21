#ifndef vector_math_h
#define vector_math_h

namespace vector_math {
	//Vector math
	vec3d  Vector_MultiplyMatrix(vec3d v, mat4x4 m);
	vec3d  Vector_CrossProduct(vec3d u, vec3d v);
	vec3d  Vector_Add(vec3d a, vec3d b);
	vec3d  Vector_Sub(vec3d a, vec3d b);
	vec3d  Vector_Mul(vec3d a, float sc);
	vec3d  Vector_Div(vec3d a, float sc);
	vec3d  Vector_Normalize(vec3d v);
	float  Vector_DotProduct(vec3d a, vec3d b);
	float  Vector_Magnitude(vec3d v);
	/////////////////MATRICES//////////////////
	//Generates
	mat4x4 GenerateTranslationMatrix(vec3d v);
	mat4x4 GenerateRotationMatrixX(float a);
	mat4x4 GenerateRotationMatrixY(float a);
	mat4x4 GenerateRotationMatrixZ(float a);
	mat4x4 GenerateIdentityMatrix();
	//Functions
	mat4x4 Matrix_MultiplyMatrix(mat4x4 a, mat4x4 b);
}

#endif
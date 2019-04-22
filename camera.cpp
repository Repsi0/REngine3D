#include "camera.h"
#include "utils.h"
#include "vector_math.h"
#include <math.h>

using namespace Utils;
using namespace vector_math;

camera::camera(float fov, float zDists[2], const int width, const int height, vec3d facing_xyz[3]) {
	fov = fov;
	zNear = zDists[0];
	zFar = zDists[1];
	aspectRatio = (float)width / (float)height;
	right = facing_xyz[0];
	up = facing_xyz[1];
	forward = facing_xyz[2];
	this->calculateProjectionMatrix();
	this->calculateViewMatrix();
}

camera::camera(float fov, const int width, const int height) {
	fov = fov;
	zNear = 0.1f;
	zFar = 1000.0f;
	aspectRatio = (float)width / (float)height;
	right = { 1,0,0 };
	up = { 0,1,0 };
	forward = { 0,0,1 };
	this->calculateProjectionMatrix();
	this->calculateViewMatrix();
}

camera::camera(const int width, const int height) {
	fov = 90.0f;
	zNear = 0.1f;
	zFar = 1000.0f;
	aspectRatio = (float)width/(float)height;
	right = {1,0,0};
	up = {0,1,0};
	forward = {0,0,1};
	this->calculateProjectionMatrix();
	this->calculateViewMatrix();
}

camera::~camera() {
	
}

//COOL FUNCTIONS
void camera::calculateProjectionMatrix() {
	float t = tanf(fov / 2.0f);
	float dF = zFar - zNear;
	float a = 1.0f / (aspectRatio*t);
	float b = 1.0f / (t);
	float c = -((zFar + zNear) / dF);
	float d = -((2.0f*zFar*zNear) / dF);
	float e = -1.0f;
	projectionMatrix = {{{a,0,0,0},{0,b,0,0},{0,0,c,d},{0,0,e,0}}};
}
void camera::calculateViewMatrix() {
	vec3d zaxis = Vector_Normalize(Vector_Sub(pos, forward));
	vec3d xaxis = Vector_Normalize(Vector_CrossProduct(up,zaxis));
	vec3d yaxis = Vector_CrossProduct(zaxis,xaxis);

	mat4x4 orientation = GenerateIdentityMatrix(); //DEBUG

	mat4x4 translation = {
		{
			{1,0,0,-pos.x},
			{0,1,0,-pos.y},
			{0,0,1,-pos.z},
			{0,0,0,1},
		}
	};
	viewMatrix = Matrix_MultiplyMatrix(orientation, translation);
}
void camera::calculateOrientationVectors() {
	//pitch (X)
	up = Vector_RotateAroundVector(up, right, rot.pitch); //y
	forward = Vector_RotateAroundVector(forward, right, rot.pitch); //z
	
	//yaw (Y)
	right = Vector_RotateAroundVector(right, up, rot.yaw); //x
	forward = Vector_RotateAroundVector(forward, up, rot.yaw); //z

	//roll (Z)
	right = Vector_RotateAroundVector(right, forward, rot.roll); //x
	up = Vector_RotateAroundVector(up, forward, rot.roll); //y
}

//GETTERS
vec3d camera::getForwardVector() {
	return forward;
}
vec3d camera::getUpVector() {
	return up;
}
vec3d camera::getRightVector() {
	return right;
}
vec3d camera::getPosition() {
	return pos;
}
//GETTERS -- MATRICES
mat4x4 camera::getProjMat() {
	return projectionMatrix;
}
mat4x4 camera::getViewMat() {
	return viewMatrix;
}

//SETTERS
void camera::setPosition(vec3d pos) {
	pos = pos;
	this->calculateViewMatrix();
}
void camera::changePosition(vec3d offset) {
	pos = vector_math::Vector_Add(pos, offset);
	this->calculateViewMatrix();
}
void camera::rotateTo(rotation rt) {
	rot.pitch = rt.pitch;
	rot.yaw = rt.yaw;
	rot.roll = rt.roll;
	this->calculateOrientationVectors();
	this->calculateViewMatrix();
}
void camera::rotateBy(rotation rt) {
	rot.pitch += rt.pitch;
	rot.yaw += rt.yaw;
	rot.roll += rt.roll;
	this->calculateOrientationVectors();
	this->calculateViewMatrix();
}
void camera::setAspectRatio(float ar) {
	aspectRatio = ar;
	this->calculateProjectionMatrix();
}
void camera::setFOV(float fov) {
	fov = fov;
	this->calculateProjectionMatrix();
}
void camera::setViewDistance(float zDists[2]) {
	zNear = zDists[0];
	zFar = zDists[1];
	this->calculateProjectionMatrix();
}

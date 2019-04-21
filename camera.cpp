#include "camera.h"
#include "utils.h"
#include "vector_math.h"
#include <math.h>

using namespace Utils;

camera::camera(float fov, float zDists[2], const int width, const int height, vec3d facing_xyz[3]) {
	fov = fov;
	zNear = zDists[0];
	zFar = zDists[1];
	aspectRatio = (float)width / (float)height;
	right = facing_xyz[0];
	up = facing_xyz[1];
	forward = facing_xyz[2];
	camera::calculateProjectionMatrix();
}

camera::camera(float fov, const int width, const int height) {
	fov = fov;
	zNear = 0.1f;
	zFar = 1000.0f;
	aspectRatio = (float)width / (float)height;
	right = { 1,0,0 };
	up = { 0,1,0 };
	forward = { 0,0,1 };
	camera::calculateProjectionMatrix();
}

camera::camera(const int width, const int height) {
	fov = 90.0f;
	zNear = 0.1f;
	zFar = 1000.0f;
	aspectRatio = (float)width/(float)height;
	right = {1,0,0};
	up = {0,1,0};
	forward = {0,0,1};
	camera::calculateProjectionMatrix();
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

}

//SETTERS
void camera::setPosition(vec3d pos) {
	pos = pos;
	camera::calculateViewMatrix();
}
void camera::changePosition(vec3d offset) {
	pos = vector_math::Vector_Add(pos, offset);
}
void camera::rotateTo(rotation rt) {
}
void camera::rotateBy(rotation rt) {
}
void camera::setAspectRatio(float ar) {
	aspectRatio = ar;
	camera::calculateProjectionMatrix();
}
void camera::setFOV(float fov) {
	fov = fov;
	camera::calculateProjectionMatrix();
}
void camera::setViewDistance(float zDists[2]) {
	zNear = zDists[0];
	zFar = zDists[1];
	camera::calculateProjectionMatrix();
}

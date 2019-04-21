#ifndef camera_h
#define camera_h
#include "utils.h"
#include "vector_math.h"
#include <math.h>

class camera {
private:
	//Translation stuff
	Utils::vec3d pos;
	//Orientation stuff
	Utils::vec3d up;
	Utils::vec3d right;
	Utils::vec3d forward;
	//Projection matrix stuff
	float fov=90.0f;
	float zNear=0.1f;
	float zFar=1000.0f;
	float aspectRatio=1.0f;
	//Matrices
	Utils::mat4x4 projectionMatrix;

	//Cool functions!
	void calculateProjectionMatrix();
	void calculateViewMatrix();
public:
	//Const/destr.
	camera(float fov, float zDists[2], const int width, const int height, Utils::vec3d facing_xyz[3]);
	camera(const int width, const int height);
	camera(float fov, const int width, const int height);
	~camera();
	
	//Getters
	Utils::vec3d getForwardVector();
	Utils::vec3d getUpVector();
	Utils::vec3d getRightVector();
	Utils::vec3d getPosition();
	//Matrix getters
	Utils::mat4x4 getProjMat();
	Utils::mat4x4 getViewMat();

	//Setters
	void setPosition(Utils::vec3d pos);
	void changePosition(Utils::vec3d offset);
	void rotateTo(Utils::rotation rt);
	void rotateBy(Utils::rotation rt);
	void setAspectRatio(float ar);
	void setFOV(float fov);
	void setViewDistance(float zDists[2]);
};

#endif
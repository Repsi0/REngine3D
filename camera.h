#ifndef camera_h
#define camera_h

using namespace Utils;

class camera {
private:
	//Translation stuff
	vec3d pos;
	//Orientation stuff
	vec3d up;
	vec3d right;
	vec3d forward;
	//Projection matrix stuff
	float fov=90.0f;
	float zNear=0.1f;
	float zFar=1000.0f;
	float aspectRatio=1.0f;
	//Matrices
	mat4x4 projectionMatrix;

	//Cool functions!
	void calculateProjectionMatrix();
	void calculateViewMatrix();
public:
	//Const/destr.
	camera(float fov, float zDists[2], const int width, const int height, vec3d facing_xyz[3]);
	camera(const int width, const int height);
	camera(float fov, const int width, const int height);
	~camera();
	
	//Getters
	vec3d getForwardVector();
	vec3d getUpVector();
	vec3d getRightVector();
	vec3d getPosition();
	//Matrix getters
	mat4x4 getProjMat();
	mat4x4 getViewMat();

	//Setters
	void setPosition(vec3d pos);
	void changePosition(vec3d offset);
	void rotateTo(rotation rt);
	void rotateBy(rotation rt);
	void setAspectRatio(float ar);
	void setFOV(float fov);
	void setViewDistance(float zDists[2]);
};

#endif
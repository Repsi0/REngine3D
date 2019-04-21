#ifndef REngine3D_h
#define REngine3D_h
#include "utils.h"
#include "camera.h"
#include "vector_math.h"
#include "drawer.h" //Used to draw 2D shapes, based on SDL.
#include <fstream>
#include <strstream>

class REngine3D {
private:
	Drawer* drawer; //2D drawing thing
	int WIDTH;
	int HEIGHT;
public:
	camera* cam;

	REngine3D(const char* TITLE, const int WIDTH_, const int HEIGHT_, float fov, float zNear, float zFar, bool &success);
	bool LoadMeshFromOBJFile(char* filename, Utils::mesh &m);
	void CleanUp();

	void ClearScr(Utils::color color);
	void RenderFinish();

	void DrawObject(Utils::gameobject* go, Utils::vec3d pos, Utils::rotation rot);
	void DrawObjects(std::vector<Utils::gameobject> gos, Utils::vec3d pos, Utils::rotation rot);
	void DrawMesh(Utils::mesh m, Utils::mat4x4 modelMatrix);
};
#endif //end if more like end life
#ifndef REngine3D_h
#define REngine3D_h
#include "utils.h"
#include "drawer.h" //Used to draw 2D shapes, based on SDL.
#include "camera.h"

class REngine3D {
private:
	Drawer* drawer;
	
public:
	int WIDTH;
	int HEIGHT;
	REngine3D(const char* TITLE, const int WIDTH_, const int HEIGHT_, bool &success);
	bool LoadMeshFromOBJFile(char* filename, Utils::mesh &m);
	void CleanUp();

	void ClearScr(Utils::color color);
	void RenderFinish();

	void DrawObject(Utils::gameobject* go, Utils::vec3d pos, Utils::rotation rot);
	void DrawObjects(std::vector<Utils::gameobject> gos, Utils::vec3d pos, Utils::rotation rot, camera* cam);
	void DrawMesh(Utils::mesh m, Utils::mat4x4 modelMatrix, camera* cam);
};
#endif
#include "REngine3D.h"
#include "utils.h"
#include "camera.h"
#include "vector_math.h"
#include "drawer.h" //Used to draw 2D shapes, based on SDL.
#include <fstream>
#include <strstream>
#include <iostream>

using namespace vector_math;
using namespace Utils;

REngine3D::REngine3D(const char* TITLE, const int WIDTH_, const int HEIGHT_, bool &success) {
	WIDTH = WIDTH_;
	HEIGHT = HEIGHT_;
	drawer = new Drawer(TITLE, WIDTH, HEIGHT, success); //set up SDL screen, drawer, etc.
}

bool REngine3D::LoadMeshFromOBJFile(char* filename, Utils::mesh &m) {
	std::ifstream f;
	f.open(filename);
	if (!f.is_open()) return false;

	std::vector<Utils::vec3d> verts;
	while (!f.eof()) {
		char line[128];
		f.getline(line, 128);
		std::strstream ss;
		ss << line;
		char junk;
		if (line[0] == 'v') {
			//Vertex
			Utils::vec3d v;
			ss >> junk;
			ss >> v.x >> v.y >> v.z;
			verts.push_back(v);
		}
		else if (line[0] == 'f') {
			//Triangle
			int f[3];
			ss >> junk >> f[0] >> f[1] >> f[2];
			m.tris.push_back({ {verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1]}, {255,255,255} });
		}
	}
	return true;
}

void REngine3D::CleanUp() {
	drawer->CleanUp();
}

void REngine3D::ClearScr(Utils::color color) {
	drawer->ClearScr(color);
}

void REngine3D::RenderFinish() {
	drawer->RenderFinish();
}

void REngine3D::DrawObject(Utils::gameobject* go, Utils::vec3d pos, Utils::rotation rot) {}

void REngine3D::DrawObjects(std::vector<Utils::gameobject> gos, Utils::vec3d pos, Utils::rotation rot, camera* cam) {
	for (const Utils::gameobject &go : gos) {
		Utils::mat4x4 rotationMatrixX = GenerateRotationMatrixX(go.rot.pitch + rot.pitch);
		Utils::mat4x4 rotationMatrixY = GenerateRotationMatrixY(go.rot.yaw + rot.yaw);
		Utils::mat4x4 rotationMatrixZ = GenerateRotationMatrixZ(go.rot.roll + rot.roll);

		Utils::mat4x4 rotationMatrixHalf = Matrix_MultiplyMatrix(rotationMatrixX, rotationMatrixY);
		Utils::mat4x4 rotationMatrix = Matrix_MultiplyMatrix(rotationMatrixHalf, rotationMatrixZ);

		Utils::mat4x4 translatedMatrix = GenerateTranslationMatrix({ go.pos.x + pos.x,go.pos.y + pos.y,go.pos.z + pos.z });

		Utils::mat4x4 transformedMatrix = Matrix_MultiplyMatrix(translatedMatrix, rotationMatrix);
		DrawMesh(go.m, transformedMatrix, cam);
	}
}
//DrawObjects function takes camera, a std::vector<gameobject>, and an offset position/rotation, extracts the transform details 
//from each gameobject and converts them into a Model Matrix, then passes (mesh, matrix, cam) into this function
void REngine3D::DrawMesh(Utils::mesh m, Utils::mat4x4 modelMatrix, camera* cam) {
	Utils::vec3d vUp = { 0,1,0 };

	Utils::mat4x4 viewMatrix = cam->getViewMat();
	mat4x4 matProj = cam->getProjMat();

	for (Utils::triangle t : m.tris) {
		Utils::triangle transformedTri, projectedTri, viewedTri;

		transformedTri.p[0] = Vector_MultiplyMatrix(t.p[0], modelMatrix);
		transformedTri.p[1] = Vector_MultiplyMatrix(t.p[1], modelMatrix);
		transformedTri.p[2] = Vector_MultiplyMatrix(t.p[2], modelMatrix);

		Utils::vec3d normal, line1, line2;
		line1 = Vector_Sub(transformedTri.p[1], transformedTri.p[0]);
		line2 = Vector_Sub(transformedTri.p[2], transformedTri.p[0]);
		normal = Vector_CrossProduct(line1, line2);
		normal = Vector_Normalize(normal);

		Utils::vec3d light_direction = { 0,0,-1 };
		light_direction = Vector_Normalize(light_direction);

		float dp = Vector_DotProduct(normal, light_direction);
		Uint8 color_ = (Uint8)(dp*255.0f);

		viewedTri.p[0] = Vector_MultiplyMatrix(transformedTri.p[0], viewMatrix);
		viewedTri.p[1] = Vector_MultiplyMatrix(transformedTri.p[1], viewMatrix);
		viewedTri.p[2] = Vector_MultiplyMatrix(transformedTri.p[2], viewMatrix);

		projectedTri.p[0] = Vector_MultiplyMatrix(viewedTri.p[0], matProj);
		projectedTri.p[1] = Vector_MultiplyMatrix(viewedTri.p[1], matProj);
		projectedTri.p[2] = Vector_MultiplyMatrix(viewedTri.p[2], matProj);

		projectedTri = { {projectedTri.p[0], projectedTri.p[1], projectedTri.p[2]}, {color_,color_,color_} };
		bool a = projectedTri.p[0].divW();
		bool b = projectedTri.p[1].divW();
		bool c = projectedTri.p[2].divW();

		if (Vector_DotProduct(cam->getForwardVector(), normal) < -0.2f) {
			if (a&&b&&c) {
				//scale into view
				projectedTri.p[0].x += 1.0f; projectedTri.p[0].y += 1.0f;
				projectedTri.p[1].x += 1.0f; projectedTri.p[1].y += 1.0f;
				projectedTri.p[2].x += 1.0f; projectedTri.p[2].y += 1.0f;
				
				projectedTri.p[0].x *= 0.5f * (float)WIDTH;
				projectedTri.p[0].y *= 0.5f * (float)HEIGHT;
				projectedTri.p[1].x *= 0.5f * (float)WIDTH;
				projectedTri.p[1].y *= 0.5f * (float)HEIGHT;
				projectedTri.p[2].x *= 0.5f * (float)WIDTH;
				projectedTri.p[2].y *= 0.5f * (float)HEIGHT;

				float zees[3] = { viewedTri.p[0].z,viewedTri.p[1].z,viewedTri.p[2].z };

				Utils::color red = { 1,0,0 };
				Utils::color green = { 0,1,0 };
				Utils::color blue = { 0,0,1 };
				Utils::color shaded = {(float) color_ ,(float)color_ ,(float)color_ };

				Utils::vertex vertices[3] = { {(int)projectedTri.p[0].x, (int)projectedTri.p[0].y,shaded},
					{(int)projectedTri.p[1].x, (int)projectedTri.p[1].y,shaded},
					{(int)projectedTri.p[2].x, (int)projectedTri.p[2].y,shaded} };

				drawer->Triangle(vertices, Utils::vec3d{ cam->getPosition().x,cam->getPosition().y,cam->getPosition().z }, cam->getForwardVector(), zees, true);
			}
		}
	}
}
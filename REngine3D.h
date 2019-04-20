#ifndef REngine3D_h
#define REngine3D_h
#include "drawer.h" //Used to draw 2D shapes, based on SDL.
#include <vector>
#include <fstream>
#include <strstream>
#include <iostream>
#include <algorithm>
#include "utils.h"

class REngine3D {
public:
	static Utils::mat4x4 GenerateProjectionMatrix(float fov, float ar, float zNear, float zFar) {
			float t = tanf(fov / 2.0f);
			float dF = zFar - zNear;
			float a = 1.0f / (ar*t);
			float b = 1.0f / (t);
			float c = -((zFar + zNear) / dF);
			float d = -((2.0f*zFar*zNear) / dF);
			float e = -1.0f;
			return { { {a,0,0,0},
					   {0,b,0,0},
					   {0,0,c,d},
					   {0,0,e,0} } }; //return Utils::mat4x4 representing a projection matrix
		}
	static Utils::mat4x4 GenerateTranslationMatrix(Utils::vec3d v) {
			return { { {1,0,0,v.x},
					   {0,1,0,v.y},
					   {0,0,1,v.z},
					   {0,0,0,  1} } }; //return Utils::mat4x4 representing a translation matrix
		}
	static Utils::mat4x4 GenerateRotationMatrixX(float a) {
			return { {{1,0,      0,       0},
					  {0,cosf(a),-sinf(a),0},
					  {0,sinf(a), cos(a), 0},
					  {0,0      , 0     , 1}} };
		}
	static Utils::mat4x4 GenerateRotationMatrixY(float a) {
			return { {{cosf(a),0,-sinf(a),0},
					  {0      ,1, 0      ,0},
					  {sinf(a),0, cosf(a),0},
					  {0      ,0, 0      ,1}} };
		}
	static Utils::mat4x4 GenerateRotationMatrixZ(float a) {
			return { {{cosf(a),-sinf(a),0,0},
					  {sinf(a), cosf(a),0,0},
					  {0      , 0      ,1,0},
					  {0      , 0      ,0,1}} };
		}
	static Utils::vec3d Vector_MultiplyMatrix(Utils::vec3d v, Utils::mat4x4 m) {
			float x = v.x * m.m[0][0] + v.y * m.m[0][1] + v.z * m.m[0][2] + v.w * m.m[0][3];
			float y = v.x * m.m[1][0] + v.y * m.m[1][1] + v.z * m.m[1][2] + v.w * m.m[1][3];
			float z = v.x * m.m[2][0] + v.y * m.m[2][1] + v.z * m.m[2][2] + v.w * m.m[2][3];
			float w = v.x * m.m[3][0] + v.y * m.m[3][1] + v.z * m.m[3][2] + v.w * m.m[3][3];
			return { x,y,z,w }; //ew nasty
		}
	static Utils::mat4x4 Matrix_MultiplyMatrix(Utils::mat4x4 a, Utils::mat4x4 b) {
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
	static Utils::vec3d Vector_CrossProduct(Utils::vec3d u, Utils::vec3d v) {
			return { u.y*v.z - u.z*v.y,
					u.z*v.x - u.x*v.z,
					u.x*v.y - u.y*v.x };
		}
	static Utils::vec3d Vector_Add(Utils::vec3d a, Utils::vec3d b) {
			return { a.x + b.x,a.y + b.y,a.z + b.z };
		}
	static Utils::vec3d Vector_Sub(Utils::vec3d a, Utils::vec3d b) {
			return { a.x - b.x,a.y - b.y,a.z - b.z };
		}
	static Utils::vec3d Vector_Mul(Utils::vec3d a, float sc) {
			return { a.x*sc,a.y*sc,a.z*sc };
		}
	static Utils::vec3d Vector_Div(Utils::vec3d a, float sc) {
			return { a.x / sc,a.y / sc,a.z / sc };
		}
	static float Vector_DotProduct(Utils::vec3d a, Utils::vec3d b) {
			return a.x*b.x + a.y*b.y + a.z*b.z;
		}
	static float Vector_Magnitude(Utils::vec3d v) {
			return sqrtf(Vector_DotProduct(v, v));
		}
	static Utils::vec3d Vector_Normalize(Utils::vec3d v) {
			float l = Vector_Magnitude(v);
			return { v.x / l,v.y / l,v.z / l };
		}


	static Utils::mat4x4 GeneratePointAtMatrix(Utils::vec3d &pos, Utils::vec3d &target, Utils::vec3d &up) {
			Utils::vec3d forward = Vector_Sub(target, pos);
			forward = Vector_Normalize(forward);

			Utils::vec3d a = Vector_Mul(forward, Vector_DotProduct(up, forward));
			Utils::vec3d up2 = Vector_Sub(up, a);
			up2 = Vector_Normalize(up2);

			Utils::vec3d right = Vector_CrossProduct(up2, forward);
			// Construct Dimensioning and Translation Matrix	
			Utils::mat4x4 matrix;
			matrix.m[0][0] = right.x;	matrix.m[0][1] = right.y;	matrix.m[0][2] = right.z;	matrix.m[0][3] = 0.0f;
			matrix.m[1][0] = up2.x;		matrix.m[1][1] = up2.y;		matrix.m[1][2] = up2.z;		matrix.m[1][3] = 0.0f;
			matrix.m[2][0] = forward.x;	matrix.m[2][1] = forward.y;	matrix.m[2][2] = forward.z;	matrix.m[2][3] = 0.0f;
			matrix.m[3][0] = pos.x;			matrix.m[3][1] = pos.y;			matrix.m[3][2] = pos.z;			matrix.m[3][3] = 1.0f;
			return matrix;

		}

	static Utils::mat4x4 Matrix_QuickInverse(Utils::mat4x4 &m) // Only for Rotation/Translation Matrices
		{
			Utils::mat4x4 matrix;
			matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
			matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
			matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
			matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
			matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
			matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
			matrix.m[3][3] = 1.0f;
			return matrix;
		}
	static Utils::mat4x4 Matrix_Identity() {
		return {
			{
				{1,0,0,0},
				{0,1,0,0},
				{0,0,1,0},
				{0,0,0,1}
			}
		};
	}
	static Utils::mat4x4 GenerateViewMatrix(Utils::vec3d pos, Utils::vec3d up, Utils::vec3d target) {
	
	}
private:
	
	
	
	struct TriangleNode {
		Utils::triangle t;
		TriangleNode* next;
	};

	

	Utils::mat4x4 matProj; //projection matrix
	Drawer* drawer; //2D drawing thing
	int WIDTH;
	int HEIGHT;
public:
	Utils::camera cam = { {0,0,0}, {0,0,1}, {0,0,0} };
	REngine3D(const char* TITLE, const int WIDTH_, const int HEIGHT_, float fov, float zNear, float zFar, bool &success) { //constructor
		WIDTH = WIDTH_;
		HEIGHT = HEIGHT_;
		matProj = GenerateProjectionMatrix(fov,(float)WIDTH/(float)HEIGHT,zNear,zFar); //generate projmat
		drawer = new Drawer(TITLE, WIDTH,HEIGHT, success); //set up SDL screen, drawer, etc.
	}
	bool LoadMeshFromOBJFile(char* filename, Utils::mesh &m) {
		std::ifstream f;
		f.open(filename);
		if(!f.is_open()) return false;
		
		std::vector<Utils::vec3d> verts;
		while (!f.eof()) {
			char line[128];
			f.getline(line,128);
			
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
				m.tris.push_back( {{verts[f[0]-1], verts[f[1]-1], verts[f[2]-1]}, {255,255,255}} );
			}
		}
		return true;
	}

	void CleanUp() {
		drawer->CleanUp();
	}

	void ClearScr(Utils::color color) {
		drawer->ClearScr(color);
	}

	void RenderFinish() {
		drawer->RenderFinish();
	}

	void DrawObject(Utils::gameobject* go, Utils::vec3d pos, Utils::rotation rot) {
		//std::cout << go->rot.pitch << std::endl;
	//	DrawMesh(go->m, Vector_Add(go->pos, pos), { go->rot.pitch + rot.pitch, go->rot.yaw + rot.yaw, go->rot.roll + rot.roll });
	}

	void DrawObjects(std::vector<Utils::gameobject> gos, Utils::vec3d pos, Utils::rotation rot) {
		for (const Utils::gameobject &go : gos) {
			Utils::mat4x4 rotationMatrixX = REngine3D::GenerateRotationMatrixX(go.rot.pitch + rot.pitch);
			Utils::mat4x4 rotationMatrixY = REngine3D::GenerateRotationMatrixY(go.rot.yaw + rot.yaw);
			Utils::mat4x4 rotationMatrixZ = REngine3D::GenerateRotationMatrixZ(go.rot.roll + rot.roll);

			Utils::mat4x4 rotationMatrixHalf = REngine3D::Matrix_MultiplyMatrix(rotationMatrixX, rotationMatrixY);
			Utils::mat4x4 rotationMatrix = REngine3D::Matrix_MultiplyMatrix(rotationMatrixHalf, rotationMatrixZ);

			Utils::mat4x4 translatedMatrix = REngine3D::GenerateTranslationMatrix({ go.pos.x + pos.x - cam.pos.x,go.pos.y + pos.y - cam.pos.y,go.pos.z + pos.z - cam.pos.z});

			Utils::mat4x4 transformedMatrix = REngine3D::Matrix_MultiplyMatrix(translatedMatrix, rotationMatrix);
			DrawMesh(go.m, transformedMatrix);
		}
	}

	void DrawMesh(Utils::mesh m, Utils::mat4x4 modelMatrix) {//draw a mesh
		Utils::vec3d vUp = {0,1,0};		

		Utils::mat4x4 viewMatrix = Matrix_Identity();

		for (Utils::triangle t : m.tris) {
			Utils::triangle transformedTri, projectedTri, viewedTri;
			
			transformedTri.p[0] = Vector_MultiplyMatrix(t.p[0], modelMatrix);
			transformedTri.p[1] = Vector_MultiplyMatrix(t.p[1], modelMatrix);
			transformedTri.p[2] = Vector_MultiplyMatrix(t.p[2], modelMatrix);

			Utils::vec3d normal,line1,line2;
			line1 = Vector_Sub(transformedTri.p[1], transformedTri.p[0]);
			line2 = Vector_Sub(transformedTri.p[2], transformedTri.p[0]);
			normal = Vector_CrossProduct(line1,line2);
			normal = Vector_Normalize(normal);

			Utils::vec3d light_direction = {0,0,-1};
			light_direction = Vector_Normalize(light_direction);

			float dp = Vector_DotProduct(normal, light_direction);
			Uint8 color_ = (Uint8)(dp*255.0f);
			
			viewedTri.p[0] = Vector_MultiplyMatrix(transformedTri.p[0], viewMatrix);
			viewedTri.p[1] = Vector_MultiplyMatrix(transformedTri.p[1], viewMatrix);
			viewedTri.p[2] = Vector_MultiplyMatrix(transformedTri.p[2], viewMatrix);

			projectedTri.p[0] = Vector_MultiplyMatrix(viewedTri.p[0], matProj);
			projectedTri.p[1] = Vector_MultiplyMatrix(viewedTri.p[1], matProj);
			projectedTri.p[2] = Vector_MultiplyMatrix(viewedTri.p[2], matProj);

			projectedTri = { {projectedTri.p[0], projectedTri.p[1], projectedTri.p[2]}, {color_,color_,color_}};

			bool a = projectedTri.p[0].divW();
			bool b = projectedTri.p[1].divW();
			bool c = projectedTri.p[2].divW();
			

			if(Vector_DotProduct(cam.look, normal) < -0.2f) {
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

					//std::cout << projectedTri.p[0].x << std::endl;

					float zees[3] = { viewedTri.p[0].z,viewedTri.p[1].z,viewedTri.p[2].z };

					Utils::color red = {1,0,0};
					Utils::color green = {0,1,0};
					Utils::color blue = {0,0,1};

					Utils::vertex vertices[3] = { {(int)projectedTri.p[0].x, (int)projectedTri.p[0].y,red},
						{(int)projectedTri.p[1].x, (int)projectedTri.p[1].y,green},
						{(int)projectedTri.p[2].x, (int)projectedTri.p[2].y,blue}};

					drawer->Triangle(vertices, Utils::vec3d{ cam.pos.x,cam.pos.y,cam.pos.z }, cam.look, zees, true);
				}
			}
		}
	}
};
#endif //end if more like end life
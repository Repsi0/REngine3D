#ifndef utils_h
#define utils_h
#include <vector>

namespace Utils {
	struct mat4x4 { //4x4 Matrix struct
		float m[4][4];
	};

	struct color {
		float r;
		float g;
		float b;
	};
	struct vertex {
		int x;
		int y;
		color c;
	};
	struct vec3d { //3D vector (w is required to multiply w/ mat4x4)
		float x = 0, y = 0, z = 0, w = 1;
		bool divW() {
			x /= w;
			y /= w;
			z /= w;
			w = 1;
			return true;

		}
	};
	struct vec2d {
		float u = 0;
		float v = 0;
		float w = 0;
	};
	struct triangle { //self explanatory
		vec3d p[3];
		Uint8 color[3];
		float depth;
	};
	struct mesh { //again, self explanatory
		std::vector<Utils::triangle> tris;
	};
	struct rotation {
		float pitch;
		float yaw;
		float roll;
	};
	struct gameobject { //mesh w/ position, rotation, all that jazz
		mesh m;
		vec3d pos;
		rotation rot;
	};
	
	struct camera {
		vec3d pos;
		vec3d look;
		rotation rot;
	};
}

#endif
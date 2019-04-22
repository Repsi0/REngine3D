#ifndef drawer_h
#define drawer_h
#include <SDL.h>
#include "utils.h"

class Drawer {
private:
	int W;
	int H;

	SDL_Surface* surf=nullptr;
	SDL_Window* window=nullptr;
	
	float* depthBuffer;

	void CreateSurface();
public:
	Drawer(const char *TITLE, const int WIDTH, const int HEIGHT, bool &success, int driver, Uint32 flags);
	Drawer(const char *TITLE, const int WIDTH, const int HEIGHT, bool &success);
	~Drawer();

	void CleanUp();

	void putPixel(int x, int y, Utils::color c);

	void ClearScr(Utils::color color);

	void RenderFinish();

	void Rectangle(int x, int y, int w, int h, Utils::color color, bool fill);

	void Line(int x0, int y0, int x1, int y1, Utils::color color);
	void Triangle(Utils::vertex V[3], Utils::vec3d pos, Utils::vec3d looking, float zees[3], bool fill);
};
#endif
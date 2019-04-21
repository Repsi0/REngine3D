#ifndef drawer_h
#define drawer_h

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
	~Drawer();

	void CleanUp();

	void putPixel(int x, int y, color c);

	void ClearScr(color color);

	void RenderFinish();

	void Rectangle(int x, int y, int w, int h, color color, bool fill);

	void Line(int x0, int y0, int x1, int y1, Utils::color color);
};
#endif
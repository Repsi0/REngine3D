#include "drawer.h"
#include <iostream>
#include <SDL.h>
#include "utils.h"

using namespace Utils;

Drawer::Drawer(const char *TITLE, const int WIDTH, const int HEIGHT, bool &success, int driver, Uint32 flags) {
	W = WIDTH;
	H = HEIGHT;
	success = false;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::printf("SDL could not be initialized! SDL ERROR: %s", SDL_GetError());
	}
	else {
		// SDL initialized correctly, proceed!
		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
		if (window == NULL) {
			std::printf("SDL could not create window! SDL ERROR: %s", SDL_GetError());
		}
		else {
			success = true;
			CreateSurface();
		}
	}
	depthBuffer = new float[WIDTH * HEIGHT];
	ClearScr({ 0,0,0 });
}

void Drawer::CreateSurface() {
	surf = SDL_GetWindowSurface(window);
}

Drawer::~Drawer() {
	CleanUp();
}

void Drawer::CleanUp() {
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Drawer::putPixel(int x, int y, color c) {
	int red = (int)(c.r*255.0f) % 255;
	int green = (int)(c.g*255.0f) % 255;
	int blue = (int)(c.b*255.0f) % 255;
	Uint32 pixel = red + (green << 8) + (blue << 16) + (255 << 24);
	if (x >= 0 && x <= surf->w - 1 && y >= 0 && y <= surf->h - 1) {
		Uint32* pixels = (Uint32 *)surf->pixels;
		pixels[(y*surf->w) + x] = pixel;
	}
}

void Drawer::ClearScr(color color) {
	for (int i = 0; i < surf->w; i++) {
		for (int j = 0; j < surf->h; j++) {
			putPixel(i, j, color);
		}
	}
	for (int i = 0; i < this->W*this->H; i++) depthBuffer[i] = 1000.0f;
}

void Drawer::RenderFinish() {
	SDL_UpdateWindowSurface(window);
}

void Drawer::Rectangle(int x, int y, int w, int h, Utils::color color, bool fill = false) {

	//Render it
	if (fill) {
		for (int i = x; i < x + w; i++) {
			for (int j = y; j < y + h; j++) {
				if (i <= surf->w - 1 && j <= surf->h - 1 && i >= 0 && j >= 0) {
					this->putPixel(i, j, color);
				}
			}
		}
	}
	else {
		for (int i = x; i < x + w; i++) {
			if (i <= surf->w - 1) {
				if (y <= surf->h - 1 && y >= 0)
					this->putPixel(i, y, color);
				if (y + h <= surf->h - 1 && y + h >= 0)
					this->putPixel(i, y + h, color);
			}
		}
		for (int i = y; i < y + h; i++) {
			if (i <= surf->h - 1) {
				if (x <= surf->w - 1 && x >= 0)
					this->putPixel(x, i, color);
				if (x + w <= surf->w - 1 && x + w >= 0)
					this->putPixel(x + w, i, color);
			}
		}
	}
}

void Drawer::Line(int x0, int y0, int x1, int y1, Utils::color color) {
	//Render it
	float m = ((float)y1 - (float)y0) / ((float)x1 - (float)x0);
	float b = (float)(y0);
	for (int x = x0; x <= x1; x++) {
		float y = m * (x - x0) + y0;
		this->putPixel((int)round(x), (int)round(y), color);
	}
}

void Drawer::Triangle(Utils::vertex V[3], Utils::vec3d pos, Utils::vec3d looking, float zees[3], bool fill = false) {
	//Create triangle

	int x1 = V[0].x;
	int y1 = V[0].y;
	int x2 = V[1].x;
	int y2 = V[1].y;
	int x3 = V[2].x;
	int y3 = V[2].y;

	//Render it
	if (!fill) {
		//Line(x1, y1, x2, y2, col);
		//Line(x2, y2, x3, y3, col);
		//Line(x3, y3, x1, y1, col);
	}
	if (fill) {
		//Ay < By < Cy
		if (y1 > y2) {
			std::swap(y1, y2);
			std::swap(x1, x2);
		}
		if (y1 > y3) {
			std::swap(y1, y3);
			std::swap(x1, x3);
		}
		if (y2 > y3) {
			std::swap(y2, y3);
			std::swap(x2, x3);
		}

		float dy1 = (float)(y2 - y1);
		float dx1 = (float)(x2 - x1);

		float dy2 = (float)(y3 - y1);
		float dx2 = (float)(x3 - x1);

		float Ax = (float)x1;
		float Bx = (float)x2;
		float Cx = (float)x3;
		float Ay = (float)y1;
		float By = (float)y2;
		float Cy = (float)y3;
		float Az = zees[0];
		float Bz = zees[1];
		float Cz = zees[2];

		float dax_step = 0, dbx_step = 0;

		if (dy1) dax_step = dx1 / (float)abs(dy1);
		if (dy2) dbx_step = dx2 / (float)abs(dy2);
		//first half...
		if (dy1) {
			for (int i = y1; i <= y2; i++) {
				int ax = (int)(x1 + (float)(i - y1) * dax_step);
				int bx = (int)(x1 + (float)(i - y1) * dbx_step);

				if (ax > bx) {
					std::swap(ax, bx);
				}


				for (int j = ax; j < bx; j++) {
					if (i >= 0 && i <= H - 1) {
						if (j >= 0 && j <= W - 1) {
							float Px = (float)j;
							float Py = (float)i;

							float numWv1_w = (By - Cy)*(Px - Cx) + (Cx - Bx)*(Py - Cy);
							float denWv1_w = (By - Cy)*(Ax - Cx) + (Cx - Bx)*(Ay - Cy);
							if (denWv1_w != 0) {
								float Wv1 = numWv1_w / denWv1_w;

								float numWv2_w = (Cy - Ay)*(Px - Cx) + (Ax - Cx)*(Py - Cy);
								float denWv2_w = denWv1_w;

								if (denWv2_w != 0) {
									float Wv2 = numWv2_w / denWv2_w;
									float Wv3 = 1.0f - Wv1 - Wv2;
									//INTERPOLATION TIME
									float Pz = Wv1 * zees[0] + Wv2 * zees[1] + Wv3 * zees[2];

									float r = (Wv1 * V[0].c.r) + (Wv2 * V[1].c.r) + (Wv3 * V[2].c.r);
									float g = (Wv1 * V[0].c.g) + (Wv2 * V[1].c.g) + (Wv3 * V[2].c.g);
									float b = (Wv1 * V[0].c.b) + (Wv2 * V[1].c.b) + (Wv3 * V[2].c.b);

									float depths = Pz;
									float depth = sqrtf((pos.x - j)*(pos.x - j) + (pos.y - i)*(pos.y - i) + (pos.z - depths)*(pos.z - depths));
									if (looking.z > 0) {
										if (depth > pos.z) {
											if (depth < this->depthBuffer[i * W + j]) {
												this->putPixel(j, i, { r,g,b });
												this->depthBuffer[i * W + j] = depth;
											}
										}
									}
									else {
										if (depth < pos.z) {
											if (depth < depthBuffer[i * W + j]) {
												putPixel(j, i, { r,g,b });
												depthBuffer[i * W + j] = depth;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		//second half
		dy1 = (float)(y3 - y2);
		dx1 = (float)(x3 - x2);

		if (dy1) dax_step = dx1 / (float)abs(dy1);
		if (dy2) dbx_step = dx2 / (float)abs(dy2);

		if (dy1) {
			for (int i = y2; i <= y3; i++) {
				int ax = (int)(x2 + (float)(i - y2) * dax_step);
				int bx = (int)(x1 + (float)(i - y1) * dbx_step);

				if (ax > bx)
				{
					std::swap(ax, bx);
				}

				for (int j = ax; j < bx; j++)
				{
					if (i >= 0 && i <= H - 1) {
						if (j >= 0 && j <= W - 1) {
							float Px = (float)j;
							float Py = (float)i;

							float numWv1_w = (By - Cy)*(Px - Cx) + (Cx - Bx)*(Py - Cy);
							float denWv1_w = (By - Cy)*(Ax - Cx) + (Cx - Bx)*(Ay - Cy);
							if (denWv1_w != 0) {
								float Wv1 = numWv1_w / denWv1_w;

								float numWv2_w = (Cy - Ay)*(Px - Cx) + (Ax - Cx)*(Py - Cy);
								float denWv2_w = denWv1_w;

								if (denWv2_w != 0) {
									float Wv2 = numWv2_w / denWv2_w;
									float Wv3 = 1.0f - Wv1 - Wv2;

									//INTERPOLATION TIME PART TWO
									float Pz = Wv1 * zees[0] + Wv2 * zees[1] + Wv3 * zees[2];
									float r = (Wv1 * V[0].c.r) + (Wv2 * V[1].c.r) + (Wv3 * V[2].c.r);
									float g = (Wv1 * V[0].c.g) + (Wv2 * V[1].c.g) + (Wv3 * V[2].c.g);
									float b = (Wv1 * V[0].c.b) + (Wv2 * V[1].c.b) + (Wv3 * V[2].c.b);

									float depths = Pz;
									float depth = sqrtf((pos.x - j)*(pos.x - j) + (pos.y - i)*(pos.y - i) + (pos.z - depths)*(pos.z - depths));
									if (looking.z > 0) {
										if (depth > pos.z) {
											if (depth < depthBuffer[i * W + j]) {
												putPixel(j, i, { r,g,b });
												depthBuffer[i * W + j] = depth;
											}
										}
									}
									else {
										if (depth < pos.z) {
											if (depth < depthBuffer[i * W + j]) {
												putPixel(j, i, { r,g,b });
												depthBuffer[i * W + j] = depth;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
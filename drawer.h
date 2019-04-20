#ifndef drawer_h
#define drawer_h
#include <iostream>
#include <SDL.h>
#include "utils.h"

namespace colors {
	Uint8 white[3] = { 255,255,255 };
	Uint8 red[3] = { 255,0,0 };
	Uint8 green[3] = { 0,255,0 };
	Uint8 blue[3] = { 0,0,255 };
	Uint8 yellow[3] = { 255,255,0 };
	Uint8 cyan[3] = { 0,255,255 };
	Uint8 magenta[3] = { 255,0,255 };
	Uint8 black[3] = {0,0,0};

	Uint32 toUint32(Uint8 color[3]) {
		Uint32 out = color[2] + (color[1] << 8) + (color[0] << 16) + (255 << 24);
		return out;
	}
}

namespace shapes {
	struct Line {
		int x1, y1, x2, y2;
	};
	struct Triangle {
		Line a,b,c;
	};
}

class Drawer {
private:
	SDL_Surface* surf=nullptr;
	SDL_Window* window=nullptr;

	void CreateSurface() {
		surf = SDL_GetWindowSurface(window);
	}
	int W;
	int H;
	float* depthBuffer;
public:
	struct rect {
		int x;
		int y;
		int w;
		int h;
	};
	Drawer(const char* TITLE, const int WIDTH, const int HEIGHT, bool &success, int driver=-1, Uint32 flags=SDL_RENDERER_ACCELERATED) {
		W = WIDTH;
		H = HEIGHT;
		success = false;
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			std::printf("SDL could not be initialized! SDL ERROR: %s", SDL_GetError());
		} else {
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
		ClearScr({0,0,0});
	}
	~Drawer() {
		CleanUp();
	}

	void CleanUp() {
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	void putPixel(int x, int y, Utils::color c) {
		int red = (int) (c.r*255.0f) % 255;
		int green = (int)(c.g*255.0f) % 255;
		int blue = (int)(c.b*255.0f) % 255;
		Uint32 pixel = red + (green << 8) + (blue << 16) + (255 << 24);
		if(x >= 0 && x <= surf->w - 1 && y >= 0 && y <= surf->h - 1) {
			Uint32* pixels = (Uint32 *)surf->pixels;
			pixels[ (y*surf->w) + x] = pixel;
		}
	}

	void ClearScr(Utils::color color) {
		for (int i = 0; i < surf->w; i++) {
			for (int j = 0; j < surf->h; j++) {
				putPixel(i,j,color);
			}
		}
		for(int i = 0; i < W*H; i++) depthBuffer[i] = 1000.0f;
	}

	void RenderFinish() {
		SDL_UpdateWindowSurface(window);
	}

	rect Rectangle(int x, int y, int w, int h, Utils::color color, bool fill=false) {
		
		//Create rectangle
		rect r;
		r.x=x;r.y=y;r.w=w;r.h=h;

		//Render it
		if (fill) {
			for (int i = x; i < x + w; i++) {
				for (int j = y; j < y + h; j++) {
					if (i <= surf->w - 1 && j <= surf->h - 1 && i >= 0 && j >= 0) {
						putPixel(i,j,color);
					}
				}
			}
		} else {
			for (int i = x; i < x + w; i++) {
				if (i <= surf->w - 1) {
					if(y <= surf->h-1 && y >= 0)
						putPixel(i, y, color);
					if (y+h <= surf->h - 1 && y+h >= 0)
						putPixel(i, y+h, color);
				}
			}
			for (int i = y; i < y + h; i++) {
				if (i <= surf->h - 1) {
					if(x <= surf->w-1 && x >= 0)
						putPixel(x, i, color);
					if(x+w <= surf->w-1 && x+w >= 0)
						putPixel(x+w, i, color);
				}
			}
		}
		return r;
	}
	
	shapes::Line Line(int x0, int y0, int x1, int y1, Utils::color color) {
		//Create line
		shapes::Line l = {x0,y0,x1,y1};

		//fix points if they don't go in the right order >:(
		//if(x1 < x0) std::swap(x0,x1);
		//if(y1 < y0) std::swap(y0,y1);
		
		//Render it
		float m = ((float)y1- (float)y0)/((float)x1- (float)x0);
		float b = (float)(y0);
		for (int x = x0; x <= x1; x++) {
			float y = m*(x-x0)+y0;
			putPixel((int)round(x),(int)round(y),color);
		}

		//return line
		return l;
	}

	void findBarycenter(Utils::vec3d A, Utils::vec3d B, Utils::vec3d C, float* u, float* v, float* w) {
		
		/*Utils::vec3d BA = REngine3D::Vector_Sub(B,A);
		Utils::vec3d CA = REngine3D::Vector_Sub(C,A);

		Utils::vec3d crossproduct
		
		float ABC_Area = ;

		
		u = CAP_Area / ABC_Area;
		v = ABP_Area / ABC_Area;
		w = BCP_Area / ABC_Area;*/
	}

	float computeDepth(int x, int y) {
		//findBarycenter();
		return 0;
	}

	shapes::Triangle Triangle(Utils::vertex V[3], Utils::vec3d pos, Utils::vec3d looking, float zees[3], bool fill=false) {
			//Create triangle
			shapes::Triangle t = { {V[0].x,V[0].y,V[1].x,V[1].y}, {V[1].x,V[1].y,V[2].x,V[2].y}, {V[2].x,V[2].y,V[0].x,V[0].y} };

			int x1 = V[0].x;
			int y1 = V[0].y;
			int x2 = V[1].x;
			int y2 = V[1].y;
			int x3 = V[2].x;
			int y3 = V[2].y;

			//Render it
			if(!fill) {
			//	Line(x1, y1, x2, y2, color);
			//	Line(x2, y2, x3, y3, color);
			//	Line(x3, y3, x1, y1, color);
			}
			if (fill) {
				//Ay < By < Cy
				if (y1 > y2) {
					std::swap(y1,y2);
					std::swap(x1, x2);
				}
				if (y1 > y3) {
					std::swap(y1, y3);
					std::swap(x1, x3);
				}
				if(y2 > y3) {
					std::swap(y2,y3);
					std::swap(x2, x3);
				}
			
				float dy1 = (float)(y2-y1);
				float dx1 = (float)(x2-x1);

				float dy2 = (float)(y3-y1);
				float dx2 = (float)(x3-x1);

				float Ax = (float) x1;
				float Bx = (float) x2;
				float Cx = (float) x3;
				float Ay = (float) y1;
				float By = (float) y2;
				float Cy = (float) y3;
				float Az = zees[0];
				float Bz = zees[1];
				float Cz = zees[2];

				float dax_step = 0, dbx_step = 0;

				if (dy1) dax_step = dx1 / (float)abs(dy1);
				if (dy2) dbx_step = dx2 / (float)abs(dy2);
				//first half...
				if(dy1) {
					for (int i = y1; i <= y2; i++) {
						int ax = (int)(x1 + (float)(i-y1) * dax_step);
						int bx = (int)(x1 + (float)(i-y1) * dbx_step);

						if (ax > bx) {
							std::swap(ax,bx);
						}


						for (int j = ax; j < bx; j++) {
							if (i >= 0 && i <= H - 1) {
								if (j >= 0 && j <= W - 1) {
									float Px = (float)j;
									float Py = (float)i;

									float numWv1_w = (By-Cy)*(Px-Cx) + (Cx-Bx)*(Py-Cy);
									float denWv1_w = (By-Cy)*(Ax-Cx) + (Cx-Bx)*(Ay-Cy);
									if(denWv1_w!=0) {
										float Wv1 = numWv1_w/denWv1_w;
										
										float numWv2_w = (Cy-Ay)*(Px-Cx)+(Ax-Cx)*(Py-Cy);
										float denWv2_w = denWv1_w;

										if(denWv2_w != 0) {
											float Wv2 = numWv2_w / denWv2_w;
											float Wv3 = 1.0f-Wv1-Wv2;
											//INTERPOLATION TIME
											float Pz = Wv1 * zees[0] + Wv2 * zees[1] + Wv3 * zees[2];
											
											float r = (Wv1 * V[0].c.r)+ (Wv2 * V[1].c.r)+ (Wv3 * V[2].c.r);
											float g = (Wv1 * V[0].c.g) + (Wv2 * V[1].c.g) + (Wv3 * V[2].c.g);
											float b = (Wv1 * V[0].c.b) + (Wv2 * V[1].c.b) + (Wv3 * V[2].c.b);

											float depths = Pz;
											float depth = sqrtf((pos.x - j)*(pos.x - j) + (pos.y-i)*(pos.y-i) + (pos.z - depths)*(pos.z-depths));
											if (looking.z > 0) {
												if (depth > pos.z) {
													if (depth < depthBuffer[i * W + j]) {
														putPixel(j, i, {r,g,b});
														depthBuffer[i * W + j] = depth;
													}
												}
											}
											else {
												if (depth < pos.z) {
													if (depth < depthBuffer[i * W + j]) {
														putPixel(j, i,{r,g,b});
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
									float Px = (float) j;
									float Py = (float) i;

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
														putPixel(j, i,{r,g,b});
														depthBuffer[i * W + j] = depth;
													}
												}
											}
											else {
												if (depth < pos.z) {
													if (depth < depthBuffer[i * W + j]) {
														putPixel(j, i, {r,g,b});
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
			return t;
		}


	
};
#endif
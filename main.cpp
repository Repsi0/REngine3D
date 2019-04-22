#include "REngine3D.h"
#include <chrono>
#include <iostream>
#include "utils.h"
#include <vector>
#include <SDL.h>
#include "camera.h"

const int WIDTH = 800;
const int HEIGHT = 600;
const char* TITLE = "Test!";
const float FOV = 90.0f;
const float ZNEAR = 0.1f;
const float ZFAR = 1000.0f;

int main(int argc, char* argv[]) {
	
	bool success;
	//Drawer drawer(TITLE, WIDTH, HEIGHT, success);
	REngine3D reng(TITLE, WIDTH, HEIGHT, success);
	camera* cam;
	cam = new camera(FOV, new float[2]{ ZNEAR,ZFAR }, WIDTH, HEIGHT, new Utils::vec3d[3]{ {1,0,0},{0,1,0},{0,0,1} });
	const char* filename = "C:\\Models\\teapot.obj";

	std::vector<Utils::gameobject> world;
	Utils::mesh cube;
	if(reng.LoadMeshFromOBJFile((char*)filename, cube)) {
		std::cout << "File loaded successfully! " <<filename << std::endl;
	} else {
		std::cout << "File failed to load! " << filename << std::endl;
	}

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			world.push_back( Utils::gameobject{cube,{20*i+10.0f,-2.0f,20*j+10.0f},{0,0,0}} );
		}
	}

	int i = 0;
	bool running=true;
	float dt = 0.1f;
	float a = 0.0f;
	if(success) {
		SDL_Event windowEvent;
		while (running) {
			std::chrono::time_point<std::chrono::system_clock> start, end;
			a += 1.0f * dt;
			start = std::chrono::system_clock::now();
			//Render
			reng.ClearScr({0,0,0});
			reng.DrawObjects(world, {0,0,0}, {0,a,0}, cam);
			//Handle events
			SDL_PollEvent(&windowEvent);
			if (windowEvent.type == SDL_QUIT) {
				running = false;
			}
			else if (windowEvent.type == SDL_KEYDOWN) {
				switch (windowEvent.key.keysym.sym) {
					case SDLK_UP:
						cam->rotateBy({1.0f*dt,0,0});
						break;
					case SDLK_DOWN:
						cam->rotateBy({ -1.0f*dt,0,0 });
						break;
					case SDLK_LEFT:
						cam->rotateBy({ 0,-1.0f*dt,0 });
						break;
					case SDLK_RIGHT:
						cam->rotateBy({ 0,1.0f*dt,0 });
						break;
					case SDLK_a:
						cam->changePosition({80.0f * dt,0,0});
						break;
					case SDLK_d:
						cam->changePosition({ -80.0f * dt,0,0 });
						break;
					case SDLK_w:
						cam->changePosition({ 0,80.0f * dt,0 });
						break;
					case SDLK_s:
						cam->changePosition({ 0,-80.0f * dt,0 });
						break;
					case SDLK_q:
						cam->changePosition({ 0,0,80.0f * dt});
						break;
					case SDLK_e:
						cam->changePosition({ 0,0,-80.0f * dt });
						break;
				}
			}
			//Show screen
			reng.RenderFinish();
			
			end = std::chrono::system_clock::now();

			std::chrono::duration<double> elapsed_seconds = end - start;
			if(i % 100 == 0) {
				std::cout <<  "elapsed time: " << elapsed_seconds.count() << "s (FPS: " << (1.0f/elapsed_seconds.count()) << ")\n";
			}
			dt = (float)elapsed_seconds.count();
			i++;
		}
	}

	running=false;
	reng.CleanUp();

	return EXIT_SUCCESS;
}
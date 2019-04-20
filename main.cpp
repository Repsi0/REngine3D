#include "REngine3D.h"
#include <chrono>

const int WIDTH = 800;
const int HEIGHT = 600;
const char* TITLE = "Test!";

int main(int argc, char* argv[]) {
	
	bool success;
	//Drawer drawer(TITLE, WIDTH, HEIGHT, success);
	REngine3D reng(TITLE, WIDTH, HEIGHT, 90.0f, 0.1f, 1000.0f, success);

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
			reng.DrawObjects(world, {0,0,0}, {0,a,0});
			//Handle events
			SDL_PollEvent(&windowEvent);
			if (windowEvent.type == SDL_QUIT) {
				running = false;
			}
			else if (windowEvent.type == SDL_KEYDOWN) {
				switch (windowEvent.key.keysym.sym) {
					case SDLK_UP:
						reng.cam.rot.pitch += 1.0f * dt;
						//reng.cam.calculateRotation();
						break;
					case SDLK_DOWN:
						reng.cam.rot.pitch -= 1.0f * dt;
						//reng.cam.calculateRotation();
						break;
					case SDLK_LEFT:
						reng.cam.rot.yaw -= 1.0f * dt;
						//reng.cam.calculateRotation();
						break;
					case SDLK_RIGHT:
						reng.cam.rot.yaw += 1.0f * dt;
						//reng.cam.calculateRotation();
						break;
					case SDLK_a:
						reng.cam.pos.x += 80.0f * dt;
						break;
					case SDLK_d:
						reng.cam.pos.x -= 80.0f * dt;
						break;
					case SDLK_w:
						reng.cam.pos.y += 80.0f * dt;
						break;
					case SDLK_s:
						reng.cam.pos.y -= 80.0f * dt;
						break;
					case SDLK_q:
						reng.cam.pos.z -= 80.0f * dt;
						break;
					case SDLK_e:
						reng.cam.pos.z += 80.0f * dt;
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
# define M_PI           3.14159265358979323846
#define _CRT_SECURE_NO_WARNINGS

#include <vector>
#include <fstream>
#include <chrono>
#include "math.h"
#include "Color.h"
#include "Shape.h"
#include "libbmp.h"
#include "world.h"

int sampleAmount = 5;
char filename[255] = "test.bmp";
double fov = .2;
int W = 1024;
int H = 768;
vec cameraPos(0, 0, 0);

Color White(255, 255, 255);
Color black(0, 0, 0);
Color Red(255, 0, 0);
vec bg(0, 0, 0.2);
vec red(1, 0, 0);
vec green(0,1,0);
vec blue(0,0,1);
vec white(1, 1, 1);


void render()
{
	const int hW = W * 0.5;
	const int hH = H * 0.5;
	fov = M_PI * fov;	// angle from center of screen to top of screen

	bmp_img img;
	bmp_img_init_df(&img, W, H);


	World world;
	Sphere* sphere1 = new Sphere(vec(0, 0, 50), 10, red);
	Sphere* sphere2 = new Sphere(vec(15, 5, 20), 5, green);
	Triangle* tri = new Triangle(vec(-5,-5,15), vec(5,-5,13), vec(0,0,10), blue);
	Sphere* light = new Sphere(vec(0, 18, 30), 6, true);
	world.addShape(sphere1);
	world.addShape(sphere2);
	world.addShape(tri);
	world.addShape(light);

	Ray r(vec(0, 0, 0), vec(0, 1, 1), std::numeric_limits<double>::max());
	
	/*vec p = r.o + (r.d * r.t);
	std::cout << box->intersect(r) << " " << r.t << std::endl;
	char trash;
	std::cin >> trash;*/

	std::cout << "Starting rendering image: " << filename << std::endl;
	time_t start = time(0);

	world.castRays(img, White, bg, white, fov, W, H, hH, hW, sampleAmount);

	std::cout << "Rendering finished in " << difftime(time(0), start) << " seconds." << std::endl;

	bmp_img_write(&img, filename);
	bmp_img_free(&img);
	
	world.free();

	const char* cmd = "start ";
	char* newArray = new char[std::strlen(cmd) + std::strlen(filename) + 1];
	std::strcpy(newArray, cmd);
	std::strcat(newArray, filename);
	system(newArray);
	delete[] newArray;
}

int main()
{
	bool cont = true;

	char choice = '0';
	while (cont)
	{
		std::cout << "Menu: " << std::endl << "  1. render image" << std::endl << "  2. define values" << std::endl << "  3. quit" << std::endl;
		char choice = '0';
		char c2 = '0';
		std::cin >> choice;
		std::cout << std::endl << std::endl;
		switch (choice) {
		case '1':
			render();
			choice = '0';
			break;
		case '2':
			choice = '0';
			c2 = '0';
			std::cout << "Values menu:" << std::endl << "  1. sample size" << std::endl << "  2. size" << std::endl << "  3. filename" << std::endl << "  4. fov" << std::endl << "  5. return" << std::endl;
			std::cin >> c2;
			switch(c2) {
			case '1':
				std::cout << "Choose sample size: ";
				std::cin >> sampleAmount;
				break;
			case '2':
				std::cout << "Choose image width: ";
				std::cin >> W;
				std::cout << "Choose image height: ";
				std::cin >> H;
				break;
			case '3':
				std::cout << "Choose file name (of less than 255 characters): ";
				std::cin >> filename;
				break;
			case '4':
				std::cout << "Please specify the camera FoV: ";
				std::cin >> fov;
				break;
			case '5':
				choice = '2';
				break;
			}
			break;
		case '3':
			cont = false;
			break;
		}
	}

	return 0;
}
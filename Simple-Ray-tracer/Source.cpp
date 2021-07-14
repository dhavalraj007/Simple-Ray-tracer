#include<iostream>
#include<chrono>

#include"stb_image.h"
#include"stb_image_write.h"
#include"glm/vec3.hpp"
#include"glm/glm.hpp"
#include"ray.h"
#include"helper.h"
#include"Camera.h"



//exports file image , encompasses pixel loop
void render(Camera&); 

//shoots ray and gets back the color of the object it hits
color shoot_Ray(const ray&);

int main()
{
	Camera camera(dpoint(0, 0, 0), 2.0, 1.0);
	auto start = std::chrono::high_resolution_clock::now();
	render(camera);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
	std::cout << "render took " << duration << " millieseconds\n";
	return 0;
}


void render(Camera& camera)
{
	unsigned char* image = new unsigned char[SCR_WIDTH * SCR_HEIGHT * SCR_NC];

	unsigned int index = 0;
	for (int y = SCR_HEIGHT - 1; y >= 0; y--)
	{
		std::cerr << "\rScan lines Remaining : " << y << "   " << std::flush;
		for (int x = 0; x < SCR_WIDTH; x++)
		{
			color col = shoot_Ray(camera.getRay(x, y));
			writeColor(image, index, col);	//takes colors in 0...1
		}
	}
	std::cerr << "\nDone.\n" << std::flush;
	stbi_write_png("./shit.png", SCR_WIDTH, SCR_HEIGHT, SCR_NC, image,
		SCR_NC * SCR_WIDTH);

}

double hit_sphere(const ray& r,dpoint center, double radii)
{
	//t2(b*b) + 2*t*b*(A-C) + (A-C)*(A-C) - r*r = 0
	dvec3 oc = r.orig - center;
	double a = dot(r.dir, r.dir);
	double half_b = dot(r.dir,oc);
	double c = dot(oc, oc) - radii * radii;
	double D = half_b * half_b - 4 * a * c;
	double t = (-half_b - sqrt(D)) / a;
	if (D < 0)
		return -1;
	else
		return t;

}


color shoot_Ray(const ray& r)
{
	double t = hit_sphere(r, { 0,0,-1 }, .3);
	if (t>0)
	{
		dpoint hitPoint = r.at(t);
		return 0.5 * (normalize(hitPoint - dvec3(0, 0, -1)) + dvec3(1.0));
	}
	double a{ 0.5 * (normalize(r.dir).y + 1.0) };
	return (1 - a) * dvec3(1.0, 1.0, 1.0) + (a)*dvec3(0.5, 0.7, 1.0);
}
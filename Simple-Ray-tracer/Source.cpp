#include<iostream>

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
	Camera camera(point(0, 0, 0), 2.0, 1.0);
	render(camera);
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

bool hit_sphere(const ray& r,dpoint center, double radii)
{
	//t2(b*b) + 2*t*b*(A-C) + (A-C)*(A-C) - r*r = 0
	glm::dvec3 oc = r.orig - center;
	double a = glm::dot(r.dir, r.dir);
	double b = glm::dot(r.dir,oc) * 2.0;
	double c = glm::dot(oc, oc) - radii * radii;
	double D = b * b - 4 * a * c;
	return D > 0;
}


color shoot_Ray(const ray& r)
{
	if (hit_sphere(r, { 0,0,-1 }, .3))
	{
		return color(1.0, 0.0, 0.0);
	}
	double a{ 0.5 * (glm::normalize(r.dir).y + 1.0) };
	return (1 - a) * glm::dvec3(1.0, 1.0, 1.0) + (a)*glm::dvec3(0.5, 0.7, 1.0);
}
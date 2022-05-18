#include<iostream>
#include<chrono>

#include"stb_image.h"
#include"stb_image_write.h"
#include"glm/vec3.hpp"
#include"glm/glm.hpp"
#include"ray.h"
#include"helper.h"
#include"Camera.h"
#include"sphere.h"
#include"hittable.h"
#include"material.h"

//exports file image , encompasses pixel loop
void render(const Camera&, const hittableList&);

//shoot ray and get back the color of the object it hit
color shoot_Ray(const ray&, const hittableList&,int);

int main()
{
	Camera camera(dpoint(0, 0, 5), 2.0, 1.0);
	hittableList worldObjects;
	auto material_ground = std::make_shared<Diffuse>(color(0.8, 0.8, 0.0));
	auto diffuseMaterial = std::make_shared<Diffuse>(color(0.7, 0.3, 0.3));
	auto metalMaterial =  std::make_shared<Metal>(color(0.8, 0.8, 0.8));

	worldObjects.add(std::make_shared<Sphere>(point(0.0, -100.5, -1.0), 100.0, material_ground));
	for (int i = 0; i < 20; i++)
	{
		auto center = vec3(i/4,i%4,3);
		std::cout << "sphere at " << center.x <<" "<< center.y << " " << center.z << std::endl;
		worldObjects.add(std::make_shared<Sphere>(center, 0.5, metalMaterial));
	}
	//worldObjects.add(std::make_shared<Sphere>(point(0.0, 0.0, -1.0), 0.5, diffuseMaterial));
	//worldObjects.add(std::make_shared<Sphere>(point(1.0, 0.0, -1.0), 0.5, metalMaterial));

	auto start = std::chrono::high_resolution_clock::now();
	render(camera, worldObjects);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
	std::cout << "render took " << duration << " millieseconds\n";
	return 0;
}

void render(const Camera& camera, const hittableList& worldObjects)
{
	unsigned char* image = new unsigned char[SCR_WIDTH * SCR_HEIGHT * SCR_NC];
	int SPP = 10;
	int depth = 10;
	unsigned int index = 0;
	for (int y = SCR_HEIGHT - 1; y >= 0; y--)
	{
		std::cerr << "\rScan lines Remaining : " << y << "   " << std::flush;
		for (int x = 0; x < SCR_WIDTH; x++)
		{
			color col(0);
			for (int s = 0; s < SPP; s++)
			{
				col += shoot_Ray(camera.getRay((x+random_double())/(SCR_WIDTH-1),(y+random_double())/(SCR_HEIGHT-1)), worldObjects,depth);
			}
			col /= double(SPP);
			writeColor(image, index, col);	//takes colors in 0...1
		}
	}
	std::cerr << "\nDone.\n" << std::flush;
	stbi_write_png("./shit1.png", SCR_WIDTH, SCR_HEIGHT, SCR_NC, image,
		SCR_NC * SCR_WIDTH);
}

color shoot_Ray(const ray& r, const hittableList& worldObjects,int depth)
{
	if (depth <= 0)
		return color(0, 0, 0);
	auto [hit, record] = worldObjects.hit(r, 0.001, infinity);
	if (hit)
	{
		color attenuation;
		ray scatteredRay;
		record.material->scatterRay(r, record, attenuation, scatteredRay);
		return attenuation*shoot_Ray(scatteredRay, worldObjects,depth-1);
	}
	//sky
	double a{ 0.5 * (normalize(r.dir).y + 1.0) };
	return (1 - a) * dvec3(1.0, 1.0, 1.0) + (a)*dvec3(0.5, 0.7, 1.0);
}
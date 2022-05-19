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
#include"lightSource.h"

//exports file image , encompasses pixel loop
void render(const Camera&, const hittableList&, const std::vector<LightSource>&);

//shoot ray and get back the color of the object it hit
color shoot_Ray(const ray&, const hittableList&,const std::vector<LightSource>&, int);
int RAY_DEPTH = 20;
int main()
{
	Camera camera(dpoint(0, 0, 1), 2.0, 1.0);
	hittableList worldObjects;
	std::vector<LightSource> lights;
	lights.emplace_back(dpoint(-1, 1, -1), color(1, 1, 1), .4f,0.3f);
	//lights.emplace_back(dpoint( 0, 1, -1), color(1, 0, 0), .4f,0.3f);
	//lights.emplace_back(dpoint( 1, 1, -1), color(0, 1, 0), .4f,0.3f);

	auto material_ground = std::make_shared<Diffuse>(color(0.8, 0.8, 0.0));
	auto diffuseMaterial = std::make_shared<Diffuse>(color(0.7, 0.3, 0.3));
	auto metalMaterial =  std::make_shared<Metal>(color(0.8, 0.8, 0.8));

	worldObjects.add(std::make_shared<Sphere>(point(0.0, -100.5, -1.0), 100.0, material_ground));
	worldObjects.add(std::make_shared<Sphere>(point(0.5, 0.0, -1.0), 0.5, metalMaterial));
	worldObjects.add(std::make_shared<Sphere>(point(-0.5, 0.0, -1.0), 0.5, diffuseMaterial));

	/*for (int i = 0; i < 20; i++)
{
	auto center = vec3(i/4,i%4,3);
	std::cout << "sphere at " << center.x <<" "<< center.y << " " << center.z << std::endl;
	worldObjects.add(std::make_shared<Sphere>(center, 0.5, metalMaterial));
}*/
	auto start = std::chrono::high_resolution_clock::now();
	render(camera, worldObjects,lights);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
	std::cout << "render took " << duration << " millieseconds\n";
	return 0;
}

void render(const Camera& camera, const hittableList& worldObjects, const std::vector<LightSource>& lights)
{
	unsigned char* image = new unsigned char[SCR_WIDTH * SCR_HEIGHT * SCR_NC];
	int SPP = 10;
	unsigned int index = 0;
	for (int y = SCR_HEIGHT - 1; y >= 0; y--)
	{
		std::cerr << "\rScan lines Remaining : " << y << "   " << std::flush;
		for (int x = 0; x < SCR_WIDTH; x++)
		{
			color col(0);
			for (int s = 0; s < SPP; s++)
			{
				col += shoot_Ray(camera.getRay((x+random_double())/(SCR_WIDTH-1),(y+random_double())/(SCR_HEIGHT-1)), worldObjects,lights,RAY_DEPTH);
			}
			col /= double(SPP);
			writeColor(image, index, col);	//takes colors in 0...1
		}
	}
	std::cerr << "\nDone.\n" << std::flush;
	stbi_write_png("./image.png", SCR_WIDTH, SCR_HEIGHT, SCR_NC, image,
		SCR_NC * SCR_WIDTH);
}

color shoot_Ray(const ray& r, const hittableList& worldObjects,const std::vector<LightSource>& lights,int depth)
{
	if (depth <= 0)				//meaning theres no light from this ray
		return color(0, 0, 0);

	auto [hit, record] = worldObjects.hit(r, 0.001, infinity);
	if (hit)
	{
		color attenuation(0);
		color lightContribution(0);
		ray scatteredRay;
		bool shouldScatter = record.material->scatterRay(r, record, attenuation, scatteredRay);

		for (auto& light : lights)
		{
			lightContribution+=light.getColor(record.hitPoint, record.surfaceNormal,worldObjects);
		}

		if (shouldScatter)
			return (attenuation )*shoot_Ray(scatteredRay, worldObjects,lights, depth - 1) + lightContribution;
		else
			return (attenuation + lightContribution);
	}
	else if(depth==RAY_DEPTH)
	{
		for (auto& light : lights)
		{
			auto [hit, record] = light.sphere->hit(r, 0.001, infinity);
			if (hit)
			{
				color attenuation(0);
				ray scatterRay;
				return light.col;
			}
		}
	}
	//if ray did not hit any object or any light then sky
	//sky
	double a{ 0.5 * (normalize(r.dir).y + 1.0) };
	return ((1 - a) * dvec3(1.0, 1.0, 1.0) + (a)*dvec3(0.44, 0.74, 0.88));
}
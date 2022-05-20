#include<iostream>
#include<chrono>

#include"stb/stb_image.h"
#include"stb/stb_image_write.h"
#include"glm/vec3.hpp"
#include"glm/glm.hpp"
#include"glm/gtc/type_ptr.hpp"

#define SDL_MAIN_HANDLED
#include"sdl2/SDL.h"
#include"world/ray.h"
#include"world/Camera.h"
#include"world/sphere.h"
#include"world/hittable.h"
#include"world/material.h"
#include"world/lightSource.h"
#include"helper.h"
#include"core/window.h"
#include"imgui/imgui.h"

//exports file image , encompasses pixel loop
unsigned char* render(const Camera&, const hittableList&, const std::vector<LightSource>&);

//shoot ray and get back the color of the object it hit
color shoot_Ray(const ray&, const hittableList&,const std::vector<LightSource>&, int);
int RAY_DEPTH = 10;
int SPP = 2;

int main()
{
	core::Window window;
	window.create(global::props);
	
	Camera camera(dpoint(0, 0, 1), 2.0, 1.0);
	hittableList worldObjects;
	std::vector<LightSource> lights;
	lights.emplace_back(dpoint(-1, 1, -1), color(1, 1, 1), .4f, 0.3f);
	//lights.emplace_back(dpoint( 0, 1, -1), color(1, 0, 0), .4f,0.3f);
	//lights.emplace_back(dpoint( 1, 1, -1), color(0, 1, 0), .4f,0.3f);

	auto material_ground = std::make_shared<Diffuse>(color(0.8, 0.8, 0.0));
	auto diffuseMaterial = std::make_shared<Diffuse>(color(0.7, 0.3, 0.3));
	auto metalMaterial = std::make_shared<Metal>(color(0.8, 0.8, 0.8));

	worldObjects.add(std::make_shared<Sphere>(point(0.0, -100.5, -1.0), 100.0, material_ground));
	worldObjects.add(std::make_shared<Sphere>(point(0.5, 0.0, -1.0), 0.5, metalMaterial));
	worldObjects.add(std::make_shared<Sphere>(point(-0.5, 0.0, -1.0), 0.5, diffuseMaterial));

	

	glm::vec3 camPos=camera.getPos();
	GLuint textureID=0;
	auto duration = 0LL;
	while (!window.m_ShouldClose)
	{
		window.beginRender();
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		auto start = std::chrono::high_resolution_clock::now();
		unsigned char* imageData = render(camera, worldObjects, lights);
		auto stop = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

		global::LoadTextureFromData(textureID, imageData, global::SCR_WIDTH, global::SCR_HEIGHT);
		//render
		ImGui::Begin("View");
		ImGui::Image((ImTextureID)textureID, ImVec2(global::SCR_WIDTH, global::SCR_HEIGHT));
		ImGui::End();

		ImGui::Begin("Controls");
		if (ImGui::Button("Render"))
		{
			auto start = std::chrono::high_resolution_clock::now();
				unsigned char* imageData = render(camera, worldObjects, lights);
			auto stop = std::chrono::high_resolution_clock::now();
			duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

			global::LoadTextureFromData(textureID, imageData, global::SCR_WIDTH, global::SCR_HEIGHT);
		}
		ImGui::Text("Last Render took: %d milliseconds", duration);
		ImGui::SliderInt("Samples per Pixel", &SPP, 1, 50);
		ImGui::SliderInt("Ray depth", &RAY_DEPTH, 1, 50);
		if (ImGui::SliderFloat3("camera pos", glm::value_ptr(camPos), -10.0, 10.0))
			camera.setPos(camPos);

		ImGui::End();
		
		
		window.endRender();
	}
	
	

	/*for (int i = 0; i < 20; i++)
{
	auto center = vec3(i/4,i%4,3);
	std::cout << "sphere at " << center.x <<" "<< center.y << " " << center.z << std::endl;
	worldObjects.add(std::make_shared<Sphere>(center, 0.5, metalMaterial));
}*/
	/*auto start = std::chrono::high_resolution_clock::now();
	unsigned char* imageData=render(camera, worldObjects,lights);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
	std::cout << "render took " << duration << " millieseconds\n";*/
	return 0;
}

unsigned char* render(const Camera& camera, const hittableList& worldObjects, const std::vector<LightSource>& lights)
{
	unsigned char* image = new unsigned char[global::SCR_WIDTH * global::SCR_HEIGHT * global::SCR_NC];

	unsigned int index = 0;
	for (int y = global::SCR_HEIGHT - 1; y >= 0; y--)
	{
		//std::cerr << "\rScan lines Remaining : " << y << "   " << std::flush;
		for (int x = 0; x < global::SCR_WIDTH; x++)
		{
			color col(0);
			for (int s = 0; s < SPP; s++)
			{
				col += shoot_Ray(camera.getRay((x+global::random_double())/(global::SCR_WIDTH-1),(y+global::random_double())/(global::SCR_HEIGHT-1)), worldObjects,lights,RAY_DEPTH);
			}
			col /= double(SPP);
			global::writeColor(image, index, col);	//takes colors in 0...1
		}
	}
	//std::cerr << "\nDone.\n" << std::flush;
	//stbi_write_png("./image.png", global::SCR_WIDTH, global::SCR_HEIGHT, global::SCR_NC, image,global::SCR_NC * global::SCR_WIDTH);
	return image;
}


//one potential wrong is that if ray hit the object then to the light it will behave as if it hit the sky so it will take sky contribution from that but it shouldnt.
color shoot_Ray(const ray& r, const hittableList& worldObjects,const std::vector<LightSource>& lights,int depth)
{
	if (depth <= 0)				//meaning theres no light from this ray
		return color(0, 0, 0);
	hitRecord record;
	auto hit = worldObjects.hit(r, 0.001, global::infinity,&record);
	if (hit)
	{
		color attenuation(0);
		color lightContribution(0);
		ray scatteredRay;
		bool shouldScatter = record.material->scatterRay(r, record, attenuation, scatteredRay);

		for (auto& light : lights)
		{
			light.getColor(lightContribution,record.hitPoint, record.surfaceNormal,worldObjects);
		}

		if (shouldScatter)
			return (attenuation)*shoot_Ray(scatteredRay, worldObjects,lights, depth - 1) + lightContribution;
		else
			return (attenuation + lightContribution);
	}
	else if(depth==RAY_DEPTH)
	{
		for (auto& light : lights)
		{
			auto hit = light.sphere->hit(r, 0.001, global::infinity,nullptr);
			if (hit)
			{
				color attenuation(0);
				return light.col;
			}
		}
	}
	//if ray did not hit any object or any light then sky
	//sky
	double a{ 0.5 * (glm::normalize(r.dir).y + 1.0) };
	return ((1 - a) * glm::dvec3(1.0, 1.0, 1.0) + (a)*glm::dvec3(0.44, 0.74, 0.88));
}
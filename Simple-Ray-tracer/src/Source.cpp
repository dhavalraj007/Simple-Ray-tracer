#include<iostream>
#include<chrono>
#include<future>

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
#include"world/plane.h"
#include"world/hittable.h"
#include"world/material.h"
#include"world/lightSource.h"
#include"helper.h"
#include"core/window.h"
#include"imgui/imgui.h"

//exports file image , encompasses pixel loop
void MT_render(const Camera& camera, const hittableList& worldObjects, const std::vector<std::shared_ptr<LightSource>>& lights, unsigned char* image);
unsigned char* render(const Camera& camera, const hittableList& worldObjects, const std::vector<std::shared_ptr<LightSource>>& lights, unsigned char* image);

//shoot ray and get back the color of the object it hit
color shoot_Ray(const ray&, const hittableList&, const std::vector<std::shared_ptr<LightSource>>&, int);
int RAY_DEPTH = 10;
int SPP = 2;

//TODO
// Bounding plane
// why white plane doesnt work
// light drawing corrections

int main()
{
	core::Window window;
	window.create(global::props);
	
	Camera camera(dpoint(0, 0, 3), 2.0, 1.0);
	hittableList worldObjects;
	std::vector<std::shared_ptr<LightSource>> lights;
	std::shared_ptr<LightSource> light1 = std::make_shared<LightSource>(dpoint(-0.5,3-0.4, 1.0), color(1, 1, 1), .8f, 0.4f);
	std::shared_ptr<LightSource> light2 = std::make_shared<LightSource>(dpoint(-1, 1, 1), color(1, 1, 1), 0.3f, 0.3f);
	lights.push_back(light1);
	worldObjects.add(light1);
	glm::vec3 lightPos=light1->center;

	auto material_ground = std::make_shared<Diffuse>(color(0.8, 0.8, 0.0));
	auto metalMaterial = std::make_shared<Metal>(color(0.8, 0.8, 0.8));
	auto diffuseMaterialRed = std::make_shared<Diffuse>(color(0.7, 0.3, 0.3));
	auto diffuseMaterialBlue = std::make_shared<Diffuse>(color(0.3, 0.3, 0.7));
	auto diffuseMaterialWhite = std::make_shared<Diffuse>(color(1, 1, 1));

	//worldObjects.add(std::make_shared<Sphere>(point(0.0, -100.5, -1.0), 100.0, material_ground));
	worldObjects.add(std::make_shared<Sphere>(point(0.5, 0.0, -1.0), 0.5, metalMaterial));
	worldObjects.add(std::make_shared<Sphere>(point(-0.5, 0.0, -1.0), 0.5, diffuseMaterialRed));
	
	//auto planeFront=std::make_shared<Plane>(point(0.0, 0.0, +3.0), glm::dvec3(0.0, 0.0, -1.0), diffuseMaterialWhite);
	auto planeBack=std::make_shared<Plane>(point(0.0, 0.0, -3.0), glm::dvec3(0.0, 0.0, 1.0), diffuseMaterialWhite);
	auto planeUp = std::make_shared<Plane>(point(0.0, 3.0, 0.0), glm::dvec3(0.0, -1.0, 0.0), diffuseMaterialWhite);
	auto planeGround = std::make_shared<Plane>(point(0.0, -0.5, 0.0), glm::dvec3(0.0, 1.0, 0.0), diffuseMaterialWhite);
	auto planeRight =std::make_shared<Plane>(point(3.0, 0.0, 0.0), glm::dvec3(-1.0, 0.0, 0.0), diffuseMaterialRed);
	auto planeLeft=std::make_shared<Plane>(point(-3.0, 0.0, 0.0), glm::dvec3(1.0, 0.0, 0.0), diffuseMaterialBlue);
	//worldObjects.add(plane1);
	worldObjects.add(planeBack);
	worldObjects.add(planeUp);
	worldObjects.add(planeGround);
	worldObjects.add(planeLeft);
	worldObjects.add(planeRight);

	//worldObjects.add(groundPlane);

	glm::vec3 camPos=camera.getPos();
	bool realTime = false;
	unsigned char* imageData = new unsigned char[global::SCR_WIDTH * global::SCR_HEIGHT * global::SCR_NC];
	memset(imageData, 0, global::SCR_WIDTH * global::SCR_HEIGHT * global::SCR_NC);
	GLuint textureID=0;
	auto duration = 0LL;
	while (!window.m_ShouldClose)
	{
		window.beginRender();
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		if (realTime)
		{
			auto start = std::chrono::high_resolution_clock::now();
			render(camera, worldObjects, lights,imageData);
			auto stop = std::chrono::high_resolution_clock::now();
			duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

			global::LoadTextureFromData(textureID, imageData, global::SCR_WIDTH, global::SCR_HEIGHT);
		}
		//render
		ImGui::Begin("View");
		ImGui::Image((ImTextureID)textureID, ImVec2(global::SCR_WIDTH, global::SCR_HEIGHT));
		ImGui::End();

		ImGui::Begin("Controls");

		if (ImGui::Button("Render"))
		{
			auto start = std::chrono::high_resolution_clock::now();
			render(camera, worldObjects, lights,imageData);
			auto stop = std::chrono::high_resolution_clock::now();
			duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

			global::LoadTextureFromData(textureID, imageData, global::SCR_WIDTH, global::SCR_HEIGHT);
		}
		
		ImGui::Text("Last Render took: %d milliseconds", duration);
		ImGui::Checkbox("Real Time", &realTime);
		ImGui::SliderInt("Samples per Pixel", &SPP, 1, 50);
		ImGui::SliderInt("Ray depth", &RAY_DEPTH, 1, 50);
		if (ImGui::SliderFloat3("camera pos", glm::value_ptr(camPos), -20.0, 20.0))
			camera.setPos(camPos);
		if (ImGui::SliderFloat3("light pos", glm::value_ptr(lightPos), -10, 10.0))
			light1->center = lightPos;
		ImGui::End();
		
		
		window.endRender();
	}

	return 0;
}

void pixelJob(const Camera& camera,const hittableList& worldObjects,const std::vector<std::shared_ptr<LightSource>>& lights, unsigned char* image,int x,int y)
{
	color col(0);
	for (int s = 0; s < SPP; s++)
	{
		col += shoot_Ray(camera.getRay((x + global::random_double()) / (global::SCR_WIDTH - 1), (y + global::random_double()) / (global::SCR_HEIGHT - 1)), worldObjects, lights, RAY_DEPTH);
	}
	col /= double(SPP);
	global::MT_writeColor(image, x, y, col);	//atomic write to image via locks //takes colors in 0...1
}

static std::vector<std::future<void>> futures;
void MT_render(const Camera& camera, const hittableList& worldObjects, const std::vector<std::shared_ptr<LightSource>>& lights,unsigned char* image)
{
	for (int y = 0; y <global::SCR_HEIGHT; y++)
	{
		for (int x = 0; x < global::SCR_WIDTH; x++)
		{
			futures.push_back(std::async(std::launch::async, pixelJob, camera, worldObjects, lights, image, x, y));
		}
	}
}

unsigned char* render(const Camera& camera, const hittableList& worldObjects, const std::vector<std::shared_ptr<LightSource>>& lights, unsigned char* image)
{
	for (int y = global::SCR_HEIGHT - 1; y >= 0; y--)
	{
		for (int x = 0; x < global::SCR_WIDTH; x++)
		{
			color col(0);
			for (int s = 0; s < SPP; s++)
			{
				double _x = (x + global::random_double()) / (global::SCR_WIDTH - 1);
				double _y = (y + global::random_double()) / (global::SCR_HEIGHT - 1);
				const ray r = camera.getRay(_x, _y);
				col += shoot_Ray(r, worldObjects, lights, RAY_DEPTH);
			}
			col /= double(SPP);
			global::writeColor(image, x,y, col);	//takes colors in 0...1
		}
	}
	return image;
}


color shoot_Ray(const ray& r, const hittableList& worldObjects,const std::vector<std::shared_ptr<LightSource>>& lights,int depth)
{
	if (depth <= 0)				//meaning theres no light from this ray
		return color(0, 0, 0);
	
	//intersect ray with world
	hitRecord record;
	if (!worldObjects.hit(r, 0.001, global::infinity, &record))
		return glm::vec3(0.2, 0.2, 0.8);	//sky
	
	//calculate direct lighting
	color directLight(0);
	for (auto& light : lights)
	{
		light->getColor(directLight, record, worldObjects);
	}

	//calculate indirect lighting
	ray scatterRay;
	bool shouldScatter = record.object->material->scatterRay(r, record,scatterRay);	
	color indirectLight = shoot_Ray(scatterRay,worldObjects,lights,depth-1);

	//surface*lighting
	if (shouldScatter)
		return record.object->material->col * (directLight+indirectLight);
	else
		return record.object->material->col * directLight;

}
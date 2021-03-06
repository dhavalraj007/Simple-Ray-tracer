#pragma once
#include<iostream>
#include<limits>
#include<random>
#include<mutex>
#include"core/window.h"
#include"glm/vec3.hpp"
#include"glm/glm.hpp"
#include"glad/glad.h"


using color = glm::vec3;
using point = glm::vec3;
using dpoint = glm::dvec3;

namespace global {

	// global vars decs
	extern core::WindowProperties props;


	// Constants
	inline constexpr auto SCR_ASPECT_RATIO = 16.0 / 9.0;
	inline constexpr unsigned int SCR_WIDTH = 1024;
	inline constexpr unsigned int SCR_HEIGHT = static_cast<unsigned int>(SCR_WIDTH / SCR_ASPECT_RATIO);
	inline constexpr auto SCR_NC = 3;
	inline constexpr double infinity = std::numeric_limits<double>::infinity();
	inline constexpr double pi = 3.1415926535897932385;

	static std::mutex s_imageMutex;

	inline void MT_writeColor(unsigned char* const image, unsigned int x,unsigned int y, color& c)
	{
		
		//std::cout << index << std::endl;
		c = { std::sqrt(c.r),sqrt(c.g),sqrt(c.b) };
		std::lock_guard<std::mutex> lock(s_imageMutex);
		auto index = (global::SCR_WIDTH * (global::SCR_HEIGHT - 1 - y) + x) * global::SCR_NC;
		image[index] = static_cast<int>(255.99 * std::clamp(c.r, 0.0f, 0.999f));
		image[index+1] = static_cast<int>(255.99 * std::clamp(c.g, 0.0f, 0.999f));
		image[index+2] = static_cast<int>(255.99 * std::clamp(c.b, 0.0f, 0.999f));
	}

	inline void writeColor(unsigned char* const image, unsigned int x, unsigned int y, color& c)
	{
		auto index = (global::SCR_WIDTH * (global::SCR_HEIGHT - 1 - y) + x) * global::SCR_NC;
		c = { std::sqrt(c.r),sqrt(c.g),sqrt(c.b) };

		image[index] = static_cast<int>(255.99 * std::clamp(c.r, 0.0f, 0.999f));
		image[index + 1] = static_cast<int>(255.99 * std::clamp(c.g, 0.0f, 0.999f));
		image[index + 2] = static_cast<int>(255.99 * std::clamp(c.b, 0.0f, 0.999f));
	}

	inline void log(const char* msg)
	{
		std::cout << msg << std::endl;
	}

	template<class c>
	inline void log(c msg)
	{
		std::cout << msg << std::cout;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns>random double between [0,1)</returns>
	inline double random_double() {
		static std::uniform_real_distribution<double> distribution(0.0, 1.0);
		static std::random_device device;
		static std::mt19937 generator(device());
		return distribution(generator);
	}

	inline double random_double(double min, double max) {
		return min + (max - min) * random_double();
	}

	inline glm::vec3 random_vec3(double min, double max)
	{
		return glm::vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}
	inline glm::vec3 random_in_unit_sphere() {
		while (true) {
			auto p = random_vec3(-1, 1);
			if (glm::length(p) >= 1) continue;
			return p;
		}
	}

	inline glm::vec3 random_in_hemisphere(const glm::vec3& normal) {
		glm::vec3 in_unit_sphere = random_in_unit_sphere();
		if (glm::dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
			return in_unit_sphere;
		else
			return -in_unit_sphere;
	}

	inline glm::dvec3 random_in_unit_disk() {
		while (true) {
			auto p = glm::dvec3(random_double(-1, 1), random_double(-1, 1), 0);
			if (glm::length(p)* glm::length(p) >= 1) continue;
			return p;
		}
	}
	struct Image {
		unsigned char* data = nullptr;
		int w, h, n;
		GLuint textureID;
	};
	
	inline bool LoadTextureFromData(GLuint& textureID,unsigned char* data,int width,int height)
	{
		if (data == nullptr)
			return false;

		// Create a OpenGL texture identifier
		if (textureID == 0)
		{
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
		}

		// Setup filtering parameters for display
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		return true;
	}
}
//auto target = (record.hitPoint) + (glm::dvec3)random_in_hemisphere(record.surfaceNormal);	//hemispherical scatter
//auto target = (record.hitPoint record.surfaceNormal) + glm::dvec3(glm::normalize(random_in_unit_sphere()));	//spherical scatter + offset normalized normal
//auto target = (record.hitPoint + record.surfaceNormal) + (random_in_unit_sphere());	//spherical scatter + offset normal
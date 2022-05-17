#pragma once
#include"glm/vec3.hpp"
#include"glm/glm.hpp"
#include<iostream>
#include<limits>
#include<random>

using namespace glm;
using color = vec3;
using point = vec3;
using dpoint = dvec3;

// Constants
inline constexpr auto SCR_ASPECT_RATIO = 16.0 / 9.0;
inline constexpr unsigned int SCR_WIDTH = 720;
inline constexpr unsigned int SCR_HEIGHT = SCR_WIDTH/SCR_ASPECT_RATIO;
inline constexpr auto SCR_NC = 3;
inline constexpr double infinity = std::numeric_limits<double>::infinity();
inline constexpr double pi = 3.1415926535897932385;
/// <summary>
///	writes color RGB to image pointer at index,index+1,index+2 also modifies index to index+3
/// </summary>
/// <param name="image"> pointer to pixel data</param>
/// <param name="index"> refrence to index where RGB is to be written</param>
/// <param name="c"> color RGB in range 0..1 </param>
inline void writeColor(unsigned char* const image,unsigned int &index,const color& c)
{
	image[index++] = static_cast<int>(255.99 * std::clamp(c.r, 0.0f,0.999f));
	image[index++] = static_cast<int>(255.99 * std::clamp(c.g, 0.0f, 0.999f));
	image[index++] = static_cast<int>(255.99 * std::clamp(c.b, 0.0f, 0.999f));
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

inline double random_double(double min,double max) {
	return min + (max - min) * random_double();
}
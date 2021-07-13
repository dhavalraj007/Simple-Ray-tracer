#pragma once
#include"glm/vec3.hpp"
#include<iostream>
using color = glm::vec3;
using point = glm::vec3;
using dpoint = glm::dvec3;
// Constants
inline constexpr auto SCR_ASPECT_RATIO = 16.0 / 9.0;
inline constexpr unsigned int SCR_WIDTH = 720;
inline constexpr unsigned int SCR_HEIGHT = SCR_WIDTH/SCR_ASPECT_RATIO;
inline constexpr auto SCR_NC = 3;

/// <summary>
///	writes color RGB to image pointer at index,index+1,index+2 also modifies index to index+3
/// </summary>
/// <param name="image"> pointer to pixel data</param>
/// <param name="index"> refrence to index where RGB is to be written</param>
/// <param name="c"> color RGB in range 0..1 </param>
void writeColor(unsigned char* const image,unsigned int &index,const color& c)
{
	image[index++] = static_cast<int>(255.99 * c.r);
	image[index++] = static_cast<int>(255.99 * c.g);
	image[index++] = static_cast<int>(255.99 * c.b);
}
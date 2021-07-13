#pragma once
#include"glm/vec3.hpp"
#include"helper.h"

class ray
{
public:
	dpoint orig;
	glm::dvec3 dir;
	
	ray() = default;
	ray(point origin,glm::dvec3 direction):orig(origin),dir(direction){}
	point at(double t) { return orig + t * dir; };
};
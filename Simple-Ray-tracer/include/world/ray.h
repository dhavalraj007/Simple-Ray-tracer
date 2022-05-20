#pragma once
#include"glm/vec3.hpp"
#include"helper.h"

class ray
{
public:
	dpoint orig;
	glm::dvec3 dir;
	
	ray() = default;
	ray(dpoint origin,glm::dvec3 direction):orig(origin),dir(direction){}
	dpoint at(double t) const { return orig + t * dir; };
};
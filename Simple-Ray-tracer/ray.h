#pragma once
#include"glm/vec3.hpp"
#include"helper.h"

class ray
{
public:
	dpoint orig;
	glm::highp_dvec3 dir;
	ray() = default;
	ray(point origin,glm::highp_dvec3 direction):orig(origin),dir(direction){}
	point at(double t) { return orig + t * dir; };
};
#pragma once
#include"glm/vec3.hpp"
#include"helper.h"

class ray
{
public:
	dpoint orig;
	dvec3 dir;
	
	ray() = default;
	ray(dpoint origin,dvec3 direction):orig(origin),dir(direction){}
	dpoint at(double t) const { return orig + t * dir; };
};
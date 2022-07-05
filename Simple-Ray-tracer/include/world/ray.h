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
bool operator==(const ray& r1, const ray& r2) {
	auto v1 = (r1.dir - r2.dir);
	auto v2 = (r1.orig- r2.orig);
	return (v1.x < 1e-7 && v1.y < 1e-7 && v1.z < 1e-7 && v2.x < 1e-7 && v2.y < 1e-7 && v2.z < 1e-7);
}
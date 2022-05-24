#pragma once
#include"ray.h"
struct hitRecord;
class Material 
{
public:
	Material(const color& c):col(c){}
	virtual bool scatterRay(const ray& r_in, const hitRecord& record, ray& scatteredRay
	) const = 0;
	color col;
};

//shoot scatter ray to get a next hit record ,compute the angle between scatter ray and normal at hit point assign propotional attenuation factor of reduction for the scatter ray hitting the current hitPoint. embedding that factor with current hitPoint's col
class Diffuse :public Material
{
public:
	Diffuse(const color& _col):Material(_col){}
	virtual bool scatterRay(const ray& r_in, const hitRecord& record, ray& scatteredRay
	) const override
	{
		scatteredRay.orig = record.hitPoint;
		scatteredRay.dir = record.hitPoint + (glm::dvec3)global::random_in_hemisphere(record.surfaceNormal);
		return true;
	}
	
};

class Metal:public Material
{
public:
	Metal(const color& _col) :Material(_col) {}
	virtual bool scatterRay(const ray& r_in, const hitRecord& record, ray& scatteredRay
	) const override
	{
		glm::dvec3 reflectedRay = glm::reflect(glm::normalize(r_in.dir), record.surfaceNormal);
		scatteredRay = ray(record.hitPoint, reflectedRay);
		return glm::dot(reflectedRay,record.surfaceNormal)>0;
	}
};

class Light :public Material
{
public:
	Light(const color& _col) :Material(_col) {}
	virtual bool scatterRay(const ray& r_in, const hitRecord& record, ray& scatteredRay
	) const override
	{

		return false;
	}
};
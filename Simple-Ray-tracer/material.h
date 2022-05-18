#pragma once
#include"ray.h"
struct hitRecord;
class Material 
{
	
public:
	virtual bool scatterRay(const ray& r_in, const hitRecord& record, color& attenuation, ray& scatteredRay
	) const = 0;
};

class Diffuse :public Material
{
public:
	Diffuse(const color& _col):col(_col){}
	virtual bool scatterRay(const ray& r_in, const hitRecord& record, color& attenuation, ray& scatteredRay
	) const override
	{
		scatteredRay = {record.hitPoint, record.hitPoint + (glm::dvec3)random_in_hemisphere(record.surfaceNormal) };
		attenuation = col;
		return true;
	}
	color col;
};

class Metal:public Material
{
public:
	Metal(const color& _col) :col(_col) {}
	virtual bool scatterRay(const ray& r_in, const hitRecord& record, color& attenuation, ray& scatteredRay
	) const override
	{
		dvec3 reflectedRay = glm::reflect(glm::normalize(r_in.dir), record.surfaceNormal);
		scatteredRay = ray(record.hitPoint, reflectedRay);
		attenuation = col;
		return glm::dot(reflectedRay,record.surfaceNormal)>0;
	}
	color col;
};
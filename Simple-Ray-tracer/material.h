#pragma once
#include"ray.h"
struct hitRecord;
class Material 
{
public:
	Material(const color& c):col(c){}
	virtual bool scatterRay(const ray& r_in, const hitRecord& record, color& attenuation, ray& scatteredRay
	) const = 0;
	color col;
};

class Diffuse :public Material
{
public:
	Diffuse(const color& _col):Material(_col){}
	virtual bool scatterRay(const ray& r_in, const hitRecord& record, color& attenuation, ray& scatteredRay
	) const override
	{
		scatteredRay = {record.hitPoint, record.hitPoint + (glm::dvec3)random_in_hemisphere(record.surfaceNormal) };
		attenuation = col;
		return true;
	}
	
};

class Metal:public Material
{
public:
	Metal(const color& _col) :Material(_col) {}
	virtual bool scatterRay(const ray& r_in, const hitRecord& record, color& attenuation, ray& scatteredRay
	) const override
	{
		dvec3 reflectedRay = glm::reflect(glm::normalize(r_in.dir), record.surfaceNormal);
		scatteredRay = ray(record.hitPoint, reflectedRay);
		attenuation = col;
		return glm::dot(reflectedRay,record.surfaceNormal)>0;
	}
};

class Light :public Material
{
public:
	Light(const color& _col) :Material(_col) {}
	virtual bool scatterRay(const ray& r_in, const hitRecord& record, color& attenuation, ray& scatteredRay
	) const override
	{
		attenuation = col * 0.2f;
		return false;
	}
};
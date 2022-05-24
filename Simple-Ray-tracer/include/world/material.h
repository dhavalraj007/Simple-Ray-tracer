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

//shoot scatter ray to get a next hit record ,compute the angle between scatter ray and normal at hit point assign propotional attenuation factor of reduction for the scatter ray hitting the current hitPoint. embedding that factor with current hitPoint's col
class Diffuse :public Material
{
public:
	Diffuse(const color& _col):Material(_col){}
	virtual bool scatterRay(const ray& r_in, const hitRecord& record, color& attenuation, ray& scatteredRay
	) const override
	{
		scatteredRay.orig = record.hitPoint;
		scatteredRay.dir = record.hitPoint + (glm::dvec3)global::random_in_hemisphere(record.surfaceNormal);

		double angle = std::acos(glm::dot(glm::normalize(scatteredRay.dir), glm::normalize(record.surfaceNormal)));
		//std::cout << angle << std::endl;
		if (angle > glm::radians(180.f) || angle < glm::radians(-180.f))	//normal is pointing away from light
		{
			//attenuation = color(0, 1, 0);
			return false;
		}
		else
		{
			//factor of reduction for next hitPoint's light -> float(1 - (angle / glm::radians(90.f)))
			if (angle > 0)
			{
				attenuation = col * std::max(0.4f,float(1 - (angle / glm::radians(180.f))));
				/*std::cout << "color was :" << col.r << " " << col.g << " " << col.b << " it became -> " << attenuation.r << " " << attenuation.g << " " << attenuation.b <<" factor= "<< float(1 - (angle / glm::radians(180.f))) <<" angle= "<<angle << std::endl;*/
			}
			else
				attenuation =  col * std::max(0.4f,float(1 - (angle / glm::radians(-180.f))));
		}
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
		glm::dvec3 reflectedRay = glm::reflect(glm::normalize(r_in.dir), record.surfaceNormal);
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
		attenuation = col ;
		return false;
	}
};
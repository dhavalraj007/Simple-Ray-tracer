#pragma once
#include"helper.h"
#include"material.h"
class LightSource:public Sphere
{
public:
	color col;
	float intensity = 1.f;
	LightSource() = default;
	LightSource(dpoint pos, color c, float i,float radii)
		:intensity(i),col(c),Sphere(pos,radii,std::make_shared<Light>(c))
	{
	}

	void getColor(color& lightContribution,const dpoint& p,const glm::dvec3& normalAtp,const hittableList& worldObjects) const
	{
		ray r(p, LightSource::center - p);	//shadow ray
		for (auto& object : worldObjects.objects)	//if shadow ray hits any objects in b/w p and light return black;
		{
			if (dynamic_cast<LightSource*>(object.get()))
				continue;
			if (object->hit(r, 0.001, 1,nullptr))
				return;
		}
		// else calculate lambert light

		double angle = std::acos(glm::dot(glm::normalize(LightSource::center - p), glm::normalize(normalAtp)));
		//std::cout << angle << std::endl;
		if (angle > glm::radians(90.f) || angle < glm::radians(-90.f))	//normal is pointing away from light
		{
			return;
		}
		else
		{
			if (angle > 0)
				lightContribution += intensity * col * float(1 - (angle / glm::radians(90.f)));
			else
				lightContribution += intensity * col * float(1 - (angle / glm::radians(-90.f)));
		}
	}
};
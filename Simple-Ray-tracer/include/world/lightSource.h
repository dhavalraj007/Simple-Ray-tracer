#pragma once
#include"helper.h"
#include"material.h"
class LightSource
{
public:
	dpoint position;
	color col;
	float intensity = 1.f;
	std::shared_ptr<Light> material;
	std::shared_ptr<Sphere> sphere;
	LightSource() = default;
	LightSource(dpoint pos, color c, float i,float radii)
		:position(pos)
		, col(c)
		, intensity(i)
	{
		sphere = std::make_shared<Sphere>(position, radii, std::make_shared<Light>(col));
	}
	color getColor(dpoint p, glm::dvec3 normalAtp,hittableList worldObjects) const
	{
		color lightContribution(0);

		ray r(p, position - p);	//shadow ray
		for (auto& object : worldObjects.objects)	//if shadow ray hits any objects in b/w p and light return black;
		{
			if (object->hit(r, 0.001, global::infinity).first)
				return lightContribution;	//0
		}
		// else calculate lambert light

		double angle = std::acos(glm::dot(glm::normalize(position - p), glm::normalize(normalAtp)));
		//std::cout << angle << std::endl;
		if (angle > glm::radians(90.f) || angle < glm::radians(-90.f))	//normal is pointing away from light
		{
			lightContribution = color(0.0, 0.0, 0.0);
		}
		else
		{
			if (angle > 0)
				lightContribution = intensity * col * float(1 - (angle / glm::radians(90.f)));
			else
				lightContribution = intensity * col * float(1 - (angle / glm::radians(-90.f)));
		}
		return lightContribution;
	}
};
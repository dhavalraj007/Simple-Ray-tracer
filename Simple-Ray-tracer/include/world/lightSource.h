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
		:intensity(i),col(c),Sphere(pos,radii,std::make_shared<Light>(c,0.0))
	{
	}

	void getColor(color& lightContribution,const hitRecord& record,const hittableList& worldObjects) const
	{
		if (dynamic_cast<LightSource*>(record.object.get()) == this)	//if calculating light to yourself
		{
			lightContribution = col;
			return;
		}

		hitRecord shadowerObjRec;
		ray r(record.hitPoint, LightSource::center - record.hitPoint);	//shadow ray
		if (worldObjects.hit(r, 0.001, 1, &shadowerObjRec))//if shadow ray hits any objects in b/w hitPoint and light 
		{
			if (shadowerObjRec.object.get() != this)	// and that obj is not lightSOurce itself
				return;							//	return black;
		}

		// else calculate lambert light
		double fact = std::max(glm::dot(glm::normalize(r.dir), glm::normalize(record.surfaceNormal)),0.0);
		lightContribution += intensity * col * float(fact);
	}
};
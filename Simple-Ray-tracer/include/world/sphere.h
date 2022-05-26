#pragma once
#include"helper.h"
#include"ray.h"
#include"hittable.h"
#include"material.h"

class Sphere:public hittable {
public:
	dpoint center;
	double radius;
	
	Sphere() = default;
	Sphere(dpoint cen,double radii,std::shared_ptr<Material> mat):center(cen),radius(radii){
		hittable::material = mat;
	}
	
	virtual bool hit(const ray& r, double t_min, double t_max, hitRecord* record) const override;
};

/// <summary>
///  calculates the hit point in the range [t_min,t_max]
/// </summary>
/// <param name="r">: ray to be checked for hit </param>
/// <param name="t_min">: lower limit of t </param>
/// <param name="t_max">: higher limit of t</param>
/// <returns>: pair of whether ray hits the sphere or not and hitRecord </returns>
bool Sphere::hit(const ray& r, double t_min, double t_max, hitRecord* record) const
{
	//t2(b*b) + 2*t*b*(A-C) + (A-C)*(A-C) - r*r = 0	// ray = A+tb , C = center , r = radius
	glm::dvec3 oc = r.orig - center;
	double a = dot(r.dir, r.dir);
	double half_b = dot(r.dir, oc);
	double c = dot(oc, oc) - radius * radius;
	double D = half_b * half_b - a * c;

	if (a < 1e-6)
		__debugbreak();
	if (D < 0)			//no hit
		return false;
	
	double t1 = (-half_b - sqrt(D)) / a;	// front hit point 
	double t2 = (-half_b + sqrt(D)) / a;	// back hit point
	double t = t1;
	if (t1<t_min || t1>t_max)
	{
		t = t2;
		if (t < t_min || t>t_max)
			return false;		// ray hit but not in the range [t_min,t_max]
	}
	//all ok
	if (record)	//if not nullptr //allowing record to be nullptr allows to optinally get record
	{
		record->hitPoint = r.at(t);
		record->surfaceNormal = (record->hitPoint - center) / radius;
		if (isnan(record->surfaceNormal.x))
			__debugbreak();

		record->t = t;
		record->set_face_normal(r);
	}
	return true;
}
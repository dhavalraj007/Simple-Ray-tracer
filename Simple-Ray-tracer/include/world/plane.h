#pragma once
#include"helper.h"
#include"ray.h"
#include"hittable.h"
#include"material.h"


class Plane :public hittable {
public:
	dpoint p;
	glm::dvec3 normal;
	std::shared_ptr<Material> material;
	Plane() = default;
	Plane(dpoint _p, const glm::vec3& _normal, std::shared_ptr<Material> mat) :p(_p), normal(_normal), material(mat) {}

	virtual bool hit(const ray& r, double t_min, double t_max, hitRecord* record) const override;
};

bool Plane::hit(const ray& r, double t_min, double t_max, hitRecord* record) const
{
	double denom = glm::dot(r.dir, normal);
	if(std::fabs(denom)<1e-6)
		return false;
	double t = glm::dot(p-r.orig, normal) / denom;
	if (t<t_min || t>t_max)
		return false;
	if (record)
	{
		record->t = t;
		record->hitPoint = r.at(t);
		record->material = material;
		record->surfaceNormal = normal;
		record->set_face_normal(r);
	}
	return true;
}
#pragma once
#include<utility>
#include<memory>
#include<vector>
#include"helper.h"
#include"ray.h"

class Material;
struct hitRecord {
	dpoint hitPoint{};
	glm::dvec3 surfaceNormal{};	//normalized
	double t{};
	bool frontFace = true;
	std::shared_ptr<Material> material;
	hitRecord() = default;
	hitRecord(dpoint _hitPoint, glm::dvec3 _surfaceNormal, double _t) :hitPoint(_hitPoint), surfaceNormal(_surfaceNormal), t(_t) {}
	hitRecord(const ray& r, dpoint _hitPoint, glm::dvec3 _surfaceNormal, double _t, std::shared_ptr<Material> _material)
		:hitPoint(_hitPoint)
		, surfaceNormal(_surfaceNormal)
		, t(_t)
		, material(_material)
	{
		set_face_normal(r);
	}
	//sets the members front_face and surfaceNormal such that surfaceNormal always points in the opposite direction of ray r.
	inline void set_face_normal(const ray& r)
	{
		frontFace = dot(r.dir, surfaceNormal) < 0;
		surfaceNormal = frontFace ? surfaceNormal : -surfaceNormal;
	}
};

class hittable {
public:
	virtual bool hit(const ray& r, double t_min, double t_max,hitRecord* record) const = 0;
};

class hittableList :public hittable {
public:
	std::vector<std::shared_ptr<hittable>> objects;

	hittableList() {}
	hittableList(std::shared_ptr<hittable> obj) { add(obj); }
	void add(std::shared_ptr<hittable> obj) { objects.push_back(obj); }
	void clear() { objects.clear(); }

	virtual bool hit(const ray& r, double t_min, double t_max, hitRecord* record) const  override;
};

bool hittableList::hit(const ray& r, double t_min, double t_max, hitRecord* record) const
{
	double closestTillNow = t_max;
	bool hitAnything = false;
	int i = 0;
	for (auto& obj : objects)
	{
		if (obj->hit(r, t_min, closestTillNow, record))
		{
			if (i == 5)
				__debugbreak();
			hitAnything = true;
			closestTillNow = record->t;
		}
		i++;
	}

	return hitAnything;
}
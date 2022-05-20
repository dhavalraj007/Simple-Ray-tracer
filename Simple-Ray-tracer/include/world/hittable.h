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
private:
	//sets the members front_face and surfaceNormal such that surfaceNormal always points in the opposite direction of ray r.
	inline void set_face_normal(const ray& r)
	{
		frontFace = dot(r.dir, surfaceNormal) < 0;
		surfaceNormal = frontFace ? surfaceNormal : -surfaceNormal;
	}
};

class hittable {
public:
	virtual std::pair<bool, hitRecord> hit(const ray& r, double t_min, double t_max) const = 0;
};

class hittableList :public hittable {
public:
	std::vector<std::shared_ptr<hittable>> objects;

	hittableList() {}
	hittableList(std::shared_ptr<hittable> obj) { add(obj); }
	void add(std::shared_ptr<hittable> obj) { objects.push_back(obj); }
	void clear() { objects.clear(); }

	virtual std::pair<bool, hitRecord> hit(const ray& r, double t_min, double t_max) const override;
};

std::pair<bool, hitRecord> hittableList::hit(const ray& r, double t_min, double t_max) const
{
	double closestTillNow = t_max;
	bool hitAnything = false;
	hitRecord record;

	for (auto obj : objects)
	{
		auto [hit, tempRecord] = obj->hit(r, t_min, closestTillNow);
		if (hit)
		{
			hitAnything = true;
			record = tempRecord;
			closestTillNow = tempRecord.t;
		}
	}

	return { hitAnything,record };
}
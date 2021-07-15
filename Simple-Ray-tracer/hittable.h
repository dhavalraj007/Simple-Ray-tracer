#pragma once
#include<utility>
#include"helper.h"
#include"ray.h"

struct hitRecord {
	dpoint hitPoint;
	dvec3 surfaceNormal;	//normalized
	double t;
	hitRecord() = default;
	hitRecord(dpoint _hitPoint,dvec3 _surfaceNormal,double _t):hitPoint(_hitPoint), surfaceNormal(_surfaceNormal),t(_t){}

};

class hittable {
public:
	virtual std::pair<bool, hitRecord> hit(const ray& r, double t_min, double t_max) = 0;
};

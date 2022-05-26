#pragma once
#include"ray.h"
struct hitRecord;
class Material 
{
public:
	Material(const color& c,double _refractiveIndex):col(c),refractiveIndex(_refractiveIndex){}
	virtual void scatterRay(const ray& r_in, const hitRecord& record, ray* &reflectedRay,ray* &refractedRay
	) const = 0;
	color col;
	//n1/n2		n1 = this medium n2 = outer medium
	double refractiveIndex;	
};


class Diffuse :public Material
{
public:
	Diffuse(const color& _col, double _refractiveIndex):Material(_col, _refractiveIndex){}
	virtual void scatterRay(const ray& r_in, const hitRecord& record, ray*& reflectedRay, ray*& refractedRay
	) const override
	{
		auto reflDir = (glm::dvec3)global::random_in_hemisphere(record.surfaceNormal);
		if (fabs(reflDir.x) < 1e-6 || fabs(reflDir.g) < 1e-6 || fabs(reflDir.b) < 1e-6)
		{
			reflDir = record.surfaceNormal;
		}
		reflectedRay= new ray(record.hitPoint,reflDir);
		refractedRay = nullptr;
	}
	
};

class Metal:public Material
{
public:
	Metal(const color& _col, double _refractiveIndex) :Material(_col, _refractiveIndex) {}
	virtual void scatterRay(const ray& r_in, const hitRecord& record, ray*& reflectedRay, ray*& refractedRay
	) const override
	{
		glm::dvec3 reflectedRayDir = glm::reflect(glm::normalize(r_in.dir), record.surfaceNormal);
		if (glm::dot(reflectedRayDir, record.surfaceNormal) > 0)
		{
			reflectedRay = new ray(record.hitPoint, reflectedRayDir);
		}
		else
			reflectedRay = nullptr;
		refractedRay = nullptr;
	}
};

//https://graphics.stanford.edu/courses/cs148-10-summer/docs/2006--degreve--reflection_refraction.pdf
glm::dvec3 reflect(const glm::dvec3& i, const glm::dvec3& n) 
{
	return i - 2.0 * (glm::dot(i, n) * n);
}
glm::dvec3 refract(const glm::dvec3& i, const glm::dvec3& n, double nr) 
{
	double cosI = -glm::dot(i, n);
	double sinI2 = glm::clamp((1.0 - cosI * cosI), 0.0001, 1.0);
	double sinT2 = nr * nr * sinI2;
	if (sinT2 > 1.0)		//TIR
		return glm::dvec3(0);
	double cosT = sqrt(1.0 - sinT2);
	return nr * i + (nr * cosI - cosT) * n;
}

double reflectance(const glm::dvec3& i, const glm::dvec3& n, double nr) 
{
	double cosI = -glm::dot(i, n);
	double sinI2 = glm::clamp((1.0 - cosI * cosI), 0.0001, 1.0);
	double sinT2 = nr * nr * sinI2;
	if (sinT2 > 1.0)		//TIR
		return 1.0;
	
	double cosT = sqrt(1.0 - sinT2);
	double rOrtho = ((nr * cosI) - cosT) / ((nr * cosI) + cosT);
	double rPar=	(cosI - (nr*cosT)) / (cosI + (nr*cosT));
	auto ret = (rOrtho * rOrtho + rPar * rPar) / 2.0;
	if (isnan(ret))
		__debugbreak();
	return ret;
}

class Dielectric:public Material
{
public:
	Dielectric(const color& _col,double _refractiveIndex) :Material(_col, _refractiveIndex){}
	
	
	virtual void scatterRay(const ray& r_in, const hitRecord& record, ray*& reflectedRay, ray*& refractedRay
	) const override
	{
		glm::dvec3 reflDir = reflect(r_in.dir, record.surfaceNormal);
		glm::dvec3 refrDir;
		if (record.frontFace)	// ray going from air to this medium
		{
			refrDir = refract(r_in.dir, record.surfaceNormal, 1.0 / refractiveIndex);
		}
		else					// ray going from this medium to air
		{
			refrDir = refract(r_in.dir, record.surfaceNormal,refractiveIndex);
		}
		reflectedRay = new ray(record.hitPoint, reflDir);
		refractedRay = new ray(record.hitPoint, refrDir);
	}
	
};

class Light :public Material
{
public:
	Light(const color& _col, double _refractiveIndex) :Material(_col, _refractiveIndex) {}
	virtual void scatterRay(const ray& r_in, const hitRecord& record, ray*& reflectedRay, ray*& refractedRay
	) const  override
	{
		reflectedRay = nullptr;
		refractedRay = nullptr;
	}
};
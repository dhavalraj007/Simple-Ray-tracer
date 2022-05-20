#pragma once
#include"helper.h"
#include"glm/vec3.hpp"
#include"ray.h"

class Camera
{
private:
	double viewport_width;
	double viewport_height;
	double focalLength;
	dpoint origin;
public:
	Camera(const dpoint& orig, double vp_width, double fLength):origin(orig),
		viewport_width(vp_width),
		viewport_height(vp_width/ global::SCR_ASPECT_RATIO ),
		focalLength(fLength){}
	void setPos(const dpoint& newPos) { origin = newPos; }
	dpoint getPos() { return origin; }
	ray getRay(double x, double y) const
	{
		//origin calncels out
		return ray(origin,
			
			- dpoint(viewport_width / 2, viewport_height / 2, focalLength)
			+ dpoint(x * viewport_width, y * viewport_height, 0)
			  );
	}

};
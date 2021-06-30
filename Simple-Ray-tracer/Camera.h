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
public:
	point origin;
	Camera(const point& orig, double vp_width, double fLength):origin(orig),
		viewport_width(vp_width),
		viewport_height(vp_width/ SCR_ASPECT_RATIO ),
		focalLength(fLength){}

	ray getRay(int x, int y)
	{
		//origin calncels out in dir
		return ray(origin,origin-
			- point(viewport_width / 2, viewport_height / 2, focalLength)
			+ point(double(x) / (SCR_WIDTH - 1.0) * viewport_width, double(y) / (SCR_HEIGHT - 1.0) * viewport_height, 0)-origin);
	}

};
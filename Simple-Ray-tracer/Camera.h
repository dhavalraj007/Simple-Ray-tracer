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
	dpoint origin;
	Camera(const dpoint& orig, double vp_width, double fLength):origin(orig),
		viewport_width(vp_width),
		viewport_height(vp_width/ SCR_ASPECT_RATIO ),
		focalLength(fLength){}

	ray getRay(int x, int y)
	{
		//origin calncels out
		return ray(origin,
			
			- dpoint(viewport_width / 2, viewport_height / 2, focalLength)
			+ dpoint(double(x) / (SCR_WIDTH - 1.0) * viewport_width, double(y) / (SCR_HEIGHT - 1.0) * viewport_height, 0)
			  );
	}

};
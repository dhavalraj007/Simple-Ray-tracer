#pragma once
#include"helper.h"
#include"glm/vec3.hpp"
#include"ray.h"

class Camera
{
public:
    Camera(
        dpoint lookfrom,
        dpoint lookat,
        glm::dvec3   vup,
        double vfov, // vertical field-of-view in degrees
        double aperture,
        double focus_dist
    ) {
        focusDistance = focus_dist;
        auto theta = glm::radians(vfov);
        auto h = tan(theta / 2);
        viewport_height = 2.0 * h;
        viewport_width = global::SCR_ASPECT_RATIO * viewport_height;

        w = glm::normalize(lookfrom - lookat);
        u = glm::normalize(glm::cross(vup, w));
        v = cross(w, u);

        origin = lookfrom;
        horizontal = focusDistance * viewport_width * u;
        vertical = focusDistance * viewport_height * v;
        lower_left_corner = origin - horizontal / 2.0 - vertical / 2.0 - focusDistance * w;

        lens_radius = aperture / 2;
    }


    ray getRay(double x, double y) const {
        glm::dvec3 rand = lens_radius * global::random_in_unit_disk();
        glm::dvec3 offset = u * rand.x + v * rand.y;

        return ray(origin+offset,
           
            lower_left_corner + x * horizontal + y* vertical -origin -offset
        );
    }
    dpoint getPos() { return origin; }
    void setPos(const dpoint& pos) {
        origin = pos;
        lower_left_corner = origin - horizontal / 2.0 - vertical / 2.0 - focusDistance * w;
    }

    void setFocusDistance(double focus_dist)
    {
        focusDistance = focus_dist;
        horizontal = focusDistance * viewport_width * u;
        vertical = focusDistance * viewport_height * v;
        lower_left_corner = origin - horizontal / 2.0 - vertical / 2.0 - focusDistance * w;
    }
private:
    dpoint origin;
    dpoint lower_left_corner;
    glm::dvec3 horizontal;
    glm::dvec3 vertical;
    glm::dvec3 u, v, w;
    double lens_radius;
    double focusDistance;
    double viewport_height;
    double viewport_width;
};


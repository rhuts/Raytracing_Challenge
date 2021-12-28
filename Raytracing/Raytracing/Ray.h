#ifndef RAY_H
#define RAY_H

#include "Vec3.h"

class Ray
{
public:
	Ray() {}
	Ray(const point3& orig, const vec3& direc)
		: origin(orig), direction(direc)
	{}

	point3	get_origin() const { return origin; }
	vec3	get_direction() const { return direction; }
	point3	at(double t) const
	{
		return origin + t * direction;
	}

	point3 origin;
	vec3 direction;
};


// Ray utility functions

inline double hit_sphere(const point3& center, double radius, const Ray& r)
{
	vec3 OC = r.get_origin() - center;
	double a = dot(r.get_direction(), r.get_direction());
	double b = 2.0 * dot(OC, r.get_direction());
	double c = dot(OC, OC) - radius * radius;
	double discriminant = b * b - 4 * a * c;

	if (discriminant < 0)
	{
		return -1.0;
	}
	else
	{
		return (-b - sqrt(discriminant)) / (2.0 * a);
	}
}

inline color ray_color(const Ray& r)
{
	double t = hit_sphere(point3(0, 0, -1), 0.5, r);

	// In front of camera
	if (t > 0.0)
	{
		vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
		return 0.5 * color(N.x() + 1, N.y() +1, N.z() + 1);
	}

	// linear interpolation
	vec3 unit_direction = unit_vector(r.get_direction());
	t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

#endif
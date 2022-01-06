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
	double a = r.get_direction().length_squared();
	double half_b = dot(OC, r.get_direction());
	double c = OC.length_squared() - radius * radius;
	double discriminant = half_b * half_b - a * c;

	if (discriminant < 0)
	{
		return -1.0;
	}
	else
	{
		return (-half_b - sqrt(discriminant)) / a;
	}
}

#endif
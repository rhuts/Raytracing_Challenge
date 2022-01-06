#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"

struct hit_record
{
	point3 p;
	vec3 normal;
	double t;
	bool front; // whether the front side was hit

	inline void set_face_normal(const Ray& r, const vec3& outward_normal) {
		front = dot(r.get_direction(), outward_normal) < 0;
		normal = front ? outward_normal : -outward_normal;
	}
};

class Hittable
{
public:
	virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif

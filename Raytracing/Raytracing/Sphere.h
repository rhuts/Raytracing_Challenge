#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"
#include "Vec3.h"

class Sphere : public Hittable
{
public:
	Sphere() {}
	Sphere(point3 c, double r) :
		center(c),
		radius(r)
	{};

	virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;

	point3 center;
	double radius;

};

bool Sphere::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 OC = r.get_origin() - center;
    double a = r.get_direction().length_squared();
    double half_b = dot(OC, r.get_direction());
    double c = OC.length_squared() - radius * radius;

    double discriminant = half_b * half_b - a * c;
    if (discriminant <= 0)
    {
        return false;
    }
    double sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range
    double root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root)
    {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
        {
            return false;
        }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);

    return true;
}

#endif
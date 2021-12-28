#ifndef CAMERA_H
#define CAMERA_H

#include "Common.h"

class Camera
{
public:
	Camera() :
		m_origin(point3(0, 0, 0)),
		m_aspect_ratio(16.0 / 9.0),
		m_viewport_height(2.0),
		m_viewport_width(m_aspect_ratio * m_viewport_height),
		m_horizontal(vec3(m_viewport_width, 0, 0)),
		m_vertical(vec3(0, m_viewport_height, 0)),
		m_focal_len(1.0),
		m_lower_left_corner(m_origin - m_horizontal / 2 - m_vertical / 2 - vec3(0, 0, m_focal_len))
	{}

	Ray get_ray(double u, double v) const
	{
		return Ray(m_origin, m_lower_left_corner + u * m_horizontal + v * m_vertical - m_origin);
	}


private:
	point3	m_origin;
	point3	m_lower_left_corner;
	vec3	m_horizontal;
	vec3	m_vertical;

	double	m_aspect_ratio;
	double	m_viewport_height;
	double	m_viewport_width;
	double	m_focal_len;
};

#endif
#ifndef RENDERER_H
#define RENDERER_H

#include <Windows.h>

#include "ErrorCheck.h"
#include "Vec3.h"
#include "Ray.h"
#include "Camera.h"


class Renderer
{
public:

	Renderer(int width) :
		m_image_width(width),
		m_aspect_ratio(16.0f / 9.0f),
		m_image_height(static_cast<int>(m_image_width / m_aspect_ratio)),
		m_viewport_height(2.0),
		m_viewport_width(m_viewport_height * m_aspect_ratio),
		m_focal_length(1.0),
		m_device_context(nullptr),
		m_frame_buffer(nullptr),
		m_frame_num(0),
		m_camera()
	{}

	RESULT		Init();
	void		CastRays(); // Cast rays and save the rasterized pixels in the frame buffer
	RESULT		Render();
	RESULT		Destroy();


	// image
	int m_image_width;
	double m_aspect_ratio;
	int m_image_height;

	// camera
	Camera m_camera;

	double m_viewport_height;
	double m_viewport_width;
	double m_focal_length;

	// display
	HDC m_device_context;
	COLORREF* m_frame_buffer;
	int m_frame_num;
};

#endif
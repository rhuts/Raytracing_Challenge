#ifndef RENDERER_H
#define RENDERER_H

#include <Windows.h>

#include "ErrorCheck.h"
#include "Camera.h"
#include "Window.h"


class Renderer
{
public:

	Renderer(int width) :
		m_image_width(width),
		m_aspect_ratio(16.0f / 9.0f),
		m_image_height(static_cast<int>(m_image_width / m_aspect_ratio)),
		m_device_context(nullptr),
		m_frame_buffer(nullptr),
		m_frame_num(0),
		m_camera(),
		m_sample_size_per_pixel(10),
		m_max_bounces(10)
	{}

	RESULT		Init();
	void		CastRays();		// Cast rays and save the rasterized pixels in the frame buffer
	void		PresentToDisplay();
	RESULT		StartRender();
	RESULT		Destroy();

	void		WritePixelAsColorRef(COLORREF& dest, color c);

private:
	// image
	int		m_image_width;
	double	m_aspect_ratio;
	int		m_image_height;

	// camera
	Camera m_camera;

	// display
	HDC				m_device_context;
	COLORREF*		m_frame_buffer;
	int				m_frame_num;
	Window*			m_window;
	//HittableList	m_scene; // TODO

	// ray tracing settings
	int m_sample_size_per_pixel;
	int m_max_bounces;
};

#endif
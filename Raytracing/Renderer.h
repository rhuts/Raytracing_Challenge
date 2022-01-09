#ifndef RENDERER_H
#define RENDERER_H

#include <Windows.h>
#include <vector>

#include "ErrorCheck.h"
#include "Camera.h"
#include "Window.h"

enum RENDER_MODE
{
	RENDER_MODE_CPU_SINGLE_THREAD	= 0,
	RENDER_MODE_CPU_MULTI_THREAD	= 1,
	RENDER_MODE_GPU_COMPUTE			= 2,
	RENDER_MODE_COUNT				= 3
};

static const char* render_mode_to_text[] =
{
	"CPU Single Thread",
	"CPU Multi Thread",
	"GPU Compute"
};

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
		m_max_bounces(10),
		m_render_mode(RENDER_MODE_CPU_SINGLE_THREAD)
	{}

	RESULT		Init();
	void		CastRays(int start_y, int end_y);		// Cast rays and save the rasterized pixels in the frame buffer
	void		PresentToDisplay();
	RESULT		StartRender();
	RESULT		Destroy();

	void		WritePixelAsColorRef(COLORREF& dest, color c);
	void		ToggleRenderMode();

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
	RENDER_MODE		m_render_mode;
	//HittableList	m_scene; // TODO

	// ray tracing settings
	int m_sample_size_per_pixel;
	int m_max_bounces;
};

#endif
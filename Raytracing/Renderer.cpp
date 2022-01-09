#include <chrono>
#include <thread>

#include "Renderer.h"
#include "Sphere.h"
#include "HittableList.h"
#include "Window.h"

using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::duration;

RESULT Renderer::Init()
{
	// Window for rendering
	m_window = new Window(m_image_width, m_image_height);
	LOG_ERROR(m_window != nullptr, "Failed to create a window for rendering.");

	// Device Context
	m_device_context = GetDC(m_window->GetHWindow());
	LOG_ERROR(m_device_context != nullptr, "Failed to get device context.");

	// Allocate frame buffer
	m_frame_buffer = (COLORREF*) calloc(m_image_width * m_image_height, sizeof(COLORREF));
	LOG_ERROR(m_frame_buffer != nullptr, "Failed to allocate memory for frame buffer.");

	m_max_threads = thread::hardware_concurrency();

	return RESULT::RES_OK;
}

inline color ray_color(const Ray& r, const HittableList& world, int bounces_remaining)
{
	hit_record rec;

	if (bounces_remaining <= 0)
	{
		return color(0, 0, 0);
	}
	else if (world.hit(r, 0.001, infinity, rec))
	{
		point3 target = rec.p + rec.normal + random_unit_vector();
		return 0.5 * ray_color(Ray(rec.p, target - rec.p), world, bounces_remaining - 1);
	}

	// linear interpolation
	vec3 unit_direction = unit_vector(r.get_direction());
	double t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

void Renderer::CastRays(int start_y, int end_y)
{
	// Timing, removed for now while implementing other render methods
	//std::chrono::steady_clock::time_point start_time = high_resolution_clock::now();

	// Create the objects in the world
	HittableList world;
	world.add(make_shared<Sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<Sphere>(point3(0, -100.5, -1), 100)); // the ground
	
	// Fill the bitmap
	for (int j = end_y - 1; j >= start_y; j--)
	{
		// Show progress for slower render methods
		if (RENDER_MODE_CPU_SINGLE_THREAD == m_render_mode)
		{
			std::cerr << "\rScanlines remaining: " << (m_image_height - j) << ' ' << "/ " << m_image_height << "\t\t\t\t" << std::flush;
		}

		for (int i = 0; i < m_image_width; i++)
		{
			color pixel_color(0, 0, 0);
			for (int k = 0; k < m_sample_size_per_pixel; k++)
			{
				double u = double(i) / (m_image_width - 1); // U from 0 to 1
				double v = double(j) / (m_image_height - 1); // V from 1 to 0

				Ray r = m_camera.get_ray(u, v);
				pixel_color += ray_color(r, world, m_max_bounces);
			}

			int64_t idx = (m_image_height - j - 1) * m_image_width + i;
			WritePixelAsColorRef(m_frame_buffer[idx], pixel_color);
		}

		// Visual aid: mark next row before rendering
		// 
		// Note: the horizontal progress lines will be incomplete for the threads
		//		 other than the presenting thread since the thread that was elected
		//		 to present will present the frame buffer before the other threads
		//		 have finished drawing their process bars
		{
			for (int i = 0; i < m_image_width && j > 0 && j > start_y; i++)
			{
				color pixel_color(1, 0, 0);
				int64_t idx = (m_image_height - j) * m_image_width + i;
				WritePixelAsColorRef(m_frame_buffer[idx], pixel_color);
			}
		}


		// all threads will be able to write the the frame buffer
		// but just one thread will be elected to present it
		if (start_y == 0)
		{
			PresentToDisplay();
			//std::cout << std::endl << "\t(" << m_frame_render_duration.count() << ") ms\n\n";
		}
	}

	// Timing
	//std::chrono::steady_clock::time_point end_time = high_resolution_clock::now();
	//duration<double, std::milli> ms_double = end_time - start_time;
	//m_frame_render_duration = ms_double;
}

void Renderer::PresentToDisplay()
{
	// Create a bitmap using the pixels in the bitmap array
	HBITMAP map = CreateBitmap(m_image_width, m_image_height, 1, 8 * 4, (void*)m_frame_buffer);
	LOG_ERROR(map != nullptr, "Failed to create bitmap.");

	// Create a temp HDC to copy the generated bitmap into
	HDC hdcSrc = CreateCompatibleDC(m_device_context);
	LOG_ERROR(hdcSrc != nullptr, "Failed to CreateCompatibleDC.");

	// Copy bitmap into HDC
	SelectObject(hdcSrc, map);

	// Copy image from the temp HDC into the destination HDC
	int res = BitBlt(m_device_context, 0, 0, m_image_width, m_image_height, hdcSrc, 0, 0, SRCCOPY);
	LOG_ERROR(res != 0, "Failed to BitBlt.");
	
	// Delete created objects
	DeleteObject(map);
	DeleteDC(hdcSrc);
}

RESULT Renderer::StartRender()
{
	// Render loop
	while (true)
	{
		switch (m_render_mode)
		{
		case RENDER_MODE_GPU_COMPUTE: // not implemented yet, fall back to CPU single threaded

		case RENDER_MODE_CPU_SINGLE_THREAD:
			CastRays(0, m_image_height);
			break;

		case RENDER_MODE_CPU_MULTI_THREAD:
			vector<thread> threads;
			for (int i = 0; i < m_max_threads; i++)
			{
				int start_y = i * ((m_image_height + m_max_threads - 1) / m_max_threads);
				int end_y = min(m_image_height - 1, start_y + (m_image_height + m_max_threads - 1) / m_max_threads);
				threads.push_back(thread(&Renderer::CastRays, this, start_y, end_y));
			}

			for (int i = 0; i < threads.size(); i++)
			{
				threads[i].join();
			}
			break;

		}

		// Exit by pressing 'Q' key
		SHORT key_state = GetAsyncKeyState(0x51);
		if (key_state & 1 || key_state & 0x80) { return RESULT::RES_OK; }

		// Switch modes with the 'M' key
		key_state = GetAsyncKeyState(0x4D);
		if (key_state & 1 || key_state & 0x80) { ToggleRenderMode(); }

		m_frame_num++;
	}

	return RESULT::RES_OK;
}

RESULT Renderer::Destroy()
{
	// Cleanup
	ReleaseDC(0, m_device_context);
	delete m_window;

	return RESULT::RES_OK;
}

void Renderer::WritePixelAsColorRef(COLORREF& dest, color c)
{
	double r = c.x();
	double g = c.y();
	double b = c.z();

	double scale = 1.0 / m_sample_size_per_pixel;
	r = sqrt(scale * r);
	g = sqrt(scale * g);
	b = sqrt(scale * b);

	int ir = static_cast<int>(256 * clamp(r, 0.0, 0.999));
	int ig = static_cast<int>(256 * clamp(g, 0.0, 0.999));
	int ib = static_cast<int>(256 * clamp(b, 0.0, 0.999));

	// Clear previous values first
	dest = 0;
	dest |= ir << 16;
	dest |= ig << 8;
	dest |= ib;
}

void Renderer::ToggleRenderMode()
{
	m_render_mode = static_cast<RENDER_MODE>((m_render_mode + 1) % RENDER_MODE_COUNT);
	printf("\nRender mode set to: (%s)\n", render_mode_to_text[m_render_mode]);
}

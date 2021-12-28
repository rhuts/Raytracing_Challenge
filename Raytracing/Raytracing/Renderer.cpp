#include <chrono>

#include "Renderer.h"
#include "Sphere.h"
#include "HittableList.h"
#include "Common.h"

using std::chrono::high_resolution_clock;
using std::chrono::duration;

RESULT Renderer::Init()
{
	// Get Desktop Device Context
	m_device_context = GetDC(0);

	// Allocate frame buffer
	m_frame_buffer = (COLORREF*) calloc(m_image_width * m_image_height, sizeof(COLORREF));
	CHECK_ERROR(m_frame_buffer != nullptr, "Failed to allocate memory for frame buffer.");

	return RESULT::RES_OK;
}

inline color ray_color(const Ray& r, const HittableList& world)
{
	hit_record rec;
	if (world.hit(r, 0, infinity, rec))
	{
		return 0.5 * (rec.normal + color(1, 1, 1));
	}

	// linear interpolation
	vec3 unit_direction = unit_vector(r.get_direction());
	double t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

void Renderer::CastRays()
{
	// Timing
	std::chrono::steady_clock::time_point start_time = high_resolution_clock::now();

	// Create the objects in the world
	HittableList world;
	world.add(make_shared<Sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<Sphere>(point3(0, -100.5, -1), 100)); // the ground

	point3 origin = point3(0, 0, 0);
	vec3 horizontal = vec3(m_viewport_width, 0, 0);
	vec3 vertical = vec3(0, m_viewport_height, 0);
	point3 lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, m_focal_length);

	// Fill the bitmap
	for (int j = m_image_height - 1; j >= 0; j--)
	{
		// Show progress
		std::cerr << "\rScanlines remaining: " << (m_image_height - j) << ' ' << "/ " << m_image_height << "\t\t\t\t" << std::flush;

		for (int i = 0; i < m_image_width; i++)
		{
			double u = double(i) / (m_image_width - 1); // U from 0 to 1
			double v = double(j) / (m_image_height - 1); // V from 1 to 0
			int64_t idx = (m_image_height - j - 1) * m_image_width + i;

			Ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
			color pixel_color = ray_color(r, world);
			write_color_as_colorref(m_frame_buffer[idx], pixel_color);
		}
	}

	// Timing
	std::chrono::steady_clock::time_point end_time = high_resolution_clock::now();
	duration<double, std::milli> ms_double = end_time - start_time;
	std::cout << std::endl << "\t(" << ms_double.count() << ") ms\n\n";
}

RESULT Renderer::Render()
{
	// Render loop
	while (true)
	{
		CastRays();

		// Create a bitmap using the pixels in the bitmap array
		HBITMAP map = CreateBitmap(m_image_width, m_image_height, 1, 8 * 4, (void*)m_frame_buffer);
		CHECK_ERROR(map != nullptr, "Failed to create bitmap.");

		// Create a temp HDC to copy the generated bitmap into
		HDC hdcSrc = CreateCompatibleDC(m_device_context);
		CHECK_ERROR(hdcSrc != nullptr, "Failed to CreateCompatibleDC.");

		// Copy bitmap into HDC
		SelectObject(hdcSrc, map);

		// Copy image from the temp HDC into the destination HDC
		int res = BitBlt(m_device_context, 2500, 10, m_image_width, m_image_height, hdcSrc, 0, 0, SRCCOPY);
		CHECK_ERROR(res != 0, "Failed to BitBlt.");

		// Exit by pressing 'Q' key
		if (GetKeyState('Q') & 0x8000/*Check if high-order bit is set (1 << 15)*/)
		{
			return RESULT::RES_OK;
		}

		m_frame_num++;
	}

	return RESULT::RES_OK;
}

RESULT Renderer::Destroy()
{
	// Cleanup
	ReleaseDC(0, m_device_context);
	//DeleteDC(hdcSrc);

	// TODO, revert desktop to old image from before overwriting

	return RESULT::RES_OK;
}
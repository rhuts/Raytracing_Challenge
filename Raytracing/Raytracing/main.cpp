// Raytracing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <thread>
#include <chrono>

// Check fails if e is false;
#define CHECK_ERROR(e, msg) \
if (!(e)) \
{std::cout << "[ERROR] " << msg << " " << __FUNCTION__ << ":" << __LINE__ << std::endl; return -1;}

void FillFrame(COLORREF *dest, int width, int height)
{
    // COLORREF is 0x00rrggbb

    // Fill the bitmap
    for (int j = height - 1; j >= 0; j--)
    {
        // Show progress
        std::cerr << "\rScanlines remaining: " << (height - j) << ' ' << "/ " << height << "\t\t\t\t" << std::flush;

        for (int i = 0; i < width; i++)
        {
            double r = double(i) / (width - 1);
            double g = double(j) / (height - 1);
            double b = 0.25;
            int ir = static_cast<int>(255 * r);
            int ig = static_cast<int>(255 * g);
            int ib = static_cast<int>(255 * b);

            int64_t idx = (height - j) * width + i;
            dest[idx] |= ir << 16;
            dest[idx] |= ig << 8;
            dest[idx] |= ib;
        }
    }
    std::cerr << "\nDone.\n";

    return;
}

int main()
{
    int width = 512;
    int height = 512;

    // Retrieve the desktop's Device Context
    HDC hdcDesktop = GetDC(0);
    CHECK_ERROR(hdcDesktop != nullptr, "Failed to get Device Context for Desktop.");

    COLORREF* bitmapArray = (COLORREF*) calloc(width * height, sizeof(COLORREF));
    CHECK_ERROR(bitmapArray != nullptr, "Failed to allocate memory for bitmap.");

    FillFrame(bitmapArray, width, height);

    // Create a bitmap using the pixels in the bitmap array
    HBITMAP map = CreateBitmap(width, height, 1, 8 * 4, (void*) bitmapArray);
    CHECK_ERROR(map != nullptr, "Failed to create bitmap.");

    // Create a temp HDC to copy the bitmap to
    HDC hdcSrc = CreateCompatibleDC(hdcDesktop);
    CHECK_ERROR(hdcSrc != nullptr, "Failed to CreateCompatibleDC.");

    // Copy bitmap into HDC
    SelectObject(hdcSrc, map);

    // Copy image from the temp HDC into the destination HDC
    while (true)
    {
        int res = BitBlt(hdcDesktop, 10, 10, width, height, hdcSrc, 0, 0, SRCCOPY);
        CHECK_ERROR(res != 0, "Failed to BitBlt.");
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Cleanup
    ReleaseDC(0, hdcDesktop);
    DeleteDC(hdcSrc);

    return 0;
}
// Raytracing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <thread>

#include "ErrorCheck.h"
#include "Vec3.h"
#include "Ray.h"
#include "Renderer.h"

/////////////////////////////////
//
// Config
// 
static const int    s_image_width = 1024;


int main()
{
    // Init Renderer
    RESULT res = RESULT::RES_OK;
    Renderer renderer(s_image_width);
    res = renderer.Init();
    CHECK_ERROR(res == RESULT::RES_OK, "Failed to Initialize Renderer.");
    
    // Start Rendering
    renderer.StartRender();

    // Clean up
    renderer.Destroy();

    return 0;
}
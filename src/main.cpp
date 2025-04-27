#include <windows.h>
#include <LogProject/Log.h>

#include "Engine.h"
#include "GraphicUtil.h"
#include <vector>

#pragma comment(linker, "/entry:WinMainCRTStartup")
#pragma comment(linker, "/subsystem:console")

#include <cmath>
#include <iostream>
#include <cfloat>

int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
    Log::Init(1024, Log::Enum::eMode_Print, Log::Enum::eLevel_Time | Log::Enum::eLevel_Type);

    Engine engine;
    if (!engine.Init(1080, 800))
    {
        return -1;
    }

    {
        std::vector<FVEC2> vertices = {
			{0.0f, 0.0f},    // 좌하단
			{100.0f, 0.0f},  // 우하단
			{100.0f, 100.0f},// 우상단
			{0.0f, 100.0f},  // 좌상단
		};

		std::vector<UINT32> indices = {
			0, 1, 2,   // 첫 번째 삼각형 (좌하단, 우하단, 우상단)
			0, 2, 3    // 두 번째 삼각형 (좌하단, 우상단, 좌상단)
		};

        Graphic::Geometry geometry = Graphic::Util::Create(vertices, indices, Graphic::eDrawMode::TRIANGLES, Graphic::ePolygonMode::FILL);
        engine.Add(geometry);        
    }

    engine.Update();

    return 0;
}
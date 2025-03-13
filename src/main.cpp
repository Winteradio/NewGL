#include <windows.h>
#include <LogProject/Log.h>

#include "Engine.h"
#include "GraphicUtil.h"
#include "EarCut.h"

#pragma comment(linker, "/entry:WinMainCRTStartup")
#pragma comment(linker, "/subsystem:console")


int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
    Log::Init(1024, Log::Enum::eMode_Print, Log::Enum::eLevel_Time | Log::Enum::eLevel_Type);

    Engine engine;
    if (!engine.Init(1080, 800))
    {
        return -1;
    }

    std::vector<FVEC2> vertices = {
        {0.0f, 0.0f},
        {2.0f, 8.0f},
        {5.0f, 10.0f},
        {8.0f, 8.0f},
        {10.0f, 0.0f},
        {7.0f, 4.0f},
        {3.0f, 4.0f}
    };

    {
        Graphic::Geometry geometry = Graphic::Util::Create(vertices, Graphic::eDrawMode::LINE_STRIP, Graphic::ePolygonMode::LINE);
        engine.Add(geometry);        
    }

    std::vector<UINT32> indices = EarCut::Triangulate(vertices);
    {
        Graphic::Geometry geometry = Graphic::Util::Create(vertices, indices, Graphic::eDrawMode::TRIANGLES, Graphic::ePolygonMode::LINE);
        engine.Add(geometry);        
    }

    engine.Update();

    return 0;
}
#include "Engine.h"

#include "glad/glad.h"
#include "glad/glad_wgl.h"

#include "Camera.h"
#include "Program.h"
#include "WindowContext.h"
#include "GLContext.h"

#include "GraphicUtil.h"

#include <LogProject/Log.h>

Engine::Engine()
	: m_spWindow(nullptr)
	, m_spContext(nullptr)
	, m_spProgram(nullptr)
	, m_spCamera(nullptr)
	, m_vMeshes()
{}

Engine::~Engine()
{
	if (!m_spProgram && 0 != m_spProgram->GetProgramID())
	{
		glDeleteProgram(m_spProgram->GetProgramID());
	}

	for (auto& mesh : m_vMeshes)
	{
		Graphic::Util::Delete(mesh);
	}
}

bool Engine::Init(const UINT32 _width, const UINT32 _height)
{
	if (!(m_spWindow = std::make_shared<WindowContext>()))
	{
		LOGERROR() << "[Engine] Failed to create Window Context";
		return false;
	}

	if (!m_spWindow->Init(_width, _height))
	{
		LOGERROR() << "[Engine] Failed to initialize Window Context";
		return false;
	}

	if (!(m_spContext = std::make_shared<GLContext>()))
	{
		LOGERROR() << "[Engine] Failed to create GL Context";
		return false;
	}

	if (!m_spContext->Init(m_spWindow->GetWindowInstance()))
	{
		LOGERROR() << "[Engine] Failed to initialize GL Context";
		return false;
	}

	ShowWindow(m_spWindow->GetWindowInstance(), SW_SHOW);
	UpdateWindow(m_spWindow->GetWindowInstance());

	if (!(m_spProgram = std::make_shared<Program>()))
	{
		LOGERROR() << "[Engine] Failed to create Program";
		return false;
	}

	std::string shaderFolder = "C:/Users/Winteradio/Desktop/WGL/shader/";
    std::string vsFileName = "SimpleColor.vs";
    std::string fsFileName = "SimpleColor.fs";

	if (!m_spProgram->SetShader(Shader::Type::VERTEX, shaderFolder + vsFileName))
	{
		LOGERROR() << "[Engine] Failed to set Vertex Shader";
		return false;
	}

	if (!m_spProgram->SetShader(Shader::Type::FRAGMENT, shaderFolder + fsFileName))
	{
		LOGERROR() << "[Engine] Failed to set Fragment Shader";
		return false;
	}

	if (!(m_spCamera = std::make_shared<Camera>()))
	{
		LOGERROR() << "[Engine] Failed to create Camera";
		return false;
	}

	FVEC3 vEye = FVEC3(0.1f, 3.0f, 0.0f);
    FVEC3 vAt = FVEC3(0.0f, -1.0f, 0.0f);
    Camera::FrustumVolume tFrust;
    tFrust.fAspect = 1080.0f / 800.f;
    tFrust.fFovY = 90.0f;
    tFrust.nFarPlane = 1000.0f;
    tFrust.nNearPlane = 0.1f;

    Camera::ViewVolume tView;
    tView.nPosX = 0;
    tView.nPoxY = 0;
    tView.nWidht = 1080;
    tView.nHeight = 800;

    m_spCamera->Init(vEye, vAt);
    m_spCamera->Init(tFrust, tView);

	if (!m_spContext->MakeCurrent())
	{
		LOGERROR() << "[Engine] Failed to make GL Context current";
		return false;
	}

	return true;
}

void Engine::Update()
{
    bool running = true;
    while (running)
    {
        MSG msg;
        while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) 
        {
            if (msg.message == WM_QUIT || msg.message == WM_CLOSE || msg.message == WM_DESTROY) 
            {
                running = false;
            }
            else 
            {
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }
        }

        Draw();
    }
}

void Engine::Add(const Graphic::Geometry& _geometry)
{
	Graphic::Mesh mesh = Graphic::Util::Create(_geometry);
	if (!Graphic::Util::Generate(_geometry, mesh))
	{
		LOGERROR() << "[Engine] Failed to generate Mesh";
		return;
	}

	m_vMeshes.push_back(mesh);
}

void Engine::Draw()
{
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto& mesh : m_vMeshes)
	{	
		glPolygonMode(GL_FRONT_AND_BACK, mesh.PolygonMode);
		glUseProgram(m_spProgram->GetProgramID());
		glBindVertexArray(mesh.ID); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawElements(mesh.DrawMode, mesh.IndexBuffer.Count, mesh.IndexBuffer.Type, nullptr);
		glBindVertexArray(0);
	}

	SwapBuffers(m_spContext->GetDeviceContext());
}
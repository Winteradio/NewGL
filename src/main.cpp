#include <windows.h>
#include "glad/glad.h"
#include "glad/glad_wgl.h"
#include <iostream>

#include "Camera.h"
#include "Parser.h"
#include "Program.h"
#include "NewWindow.h"
#include "WGLContext.h"

#include <memory>
#include <sstream>
#include <vector>

#pragma comment(linker, "/entry:WinMainCRTStartup")
#pragma comment(linker, "/subsystem:console")

unsigned int VBO, VAO;
unsigned int EBO;

void Triangle()
{
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float positions[] = {
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, -1.0f,
        -1.0f, 0.0f, -1.0f,
        -1.0f, 0.0f, 1.0f,
    }; 

    float colors[] = {
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
    };

    unsigned short indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(colors), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(colors), sizeof(positions), (void*)positions);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(colors), (void*)colors);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), (void*)indices);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)sizeof(colors));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); 
}

void Frame(const UINT64 _programID, const std::shared_ptr<Camera> _spCamera)
{
    static float color = 0.0f;

    if (color > 1.0f)
    {
        color = 0.0f;
    }

    color += 0.0001f;
    glClearColor( color, 1.0f - color, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const auto tFrust = _spCamera->GetFrustumVolume();
    const auto tView = _spCamera->GetViewVolume();

    //glViewport(tView.nPosX, tView.nPoxY, tView.nWidht, tView.nHeight);

    const GLint nModel = glGetUniformLocation(_programID, "mModel");
    const GLint nView = glGetUniformLocation(_programID, "mView");
    const GLint nProj = glGetUniformLocation(_programID, "mProj");

    const FMAT4 mModel(1.0f);
    const FMAT4 mView = _spCamera->GetViewMatrix();
    const FMAT4 mProj = _spCamera->GetProjMatrix();

    // const auto mOtherProj = glm::perspective(tFrust.fFovY, tFrust.fAspect, tFrust.nNearPlane, tFrust.nFarPlane);
    glUniformMatrix4fv(nModel, 1, false, glm::value_ptr(mModel));
    glUniformMatrix4fv(nView, 1, false, glm::value_ptr(mView));
    glUniformMatrix4fv(nProj, 1, false, glm::value_ptr(mProj));
    // glGetUniformLocation()
    // glUniformMatrix4fv()
    glUseProgram(_programID);
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);
}

int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
    std::shared_ptr<NewWindow> m_spWindow = std::make_shared<NewWindow>();
    std::shared_ptr<IContext> m_spContext = std::make_shared<WGLContext>();

    if (!m_spWindow->Create(800,600))
    {
        return -1;
    }

    if (!m_spContext->Init(m_spWindow->GetDeviceContext()))
    {
        return -1;
    }

    ShowWindow(m_spWindow->GetWindowInstance(), SW_SHOW);
    UpdateWindow(m_spWindow->GetWindowInstance());

    std::shared_ptr<Program> m_spProgram = std::make_shared<Program>();

    std::string shaderFolder = "C:/Users/Winteradio/Desktop/WGL/shader/";
    std::string vsFileName = "SimpleColor.vs";
    std::string fsFileName = "SimpleColor.fs";

    m_spProgram->SetShader(Shader::Type::VERTEX ,shaderFolder + vsFileName);
    m_spProgram->SetShader(Shader::Type::FRAGMENT, shaderFolder + fsFileName);

    std::shared_ptr<Camera> m_spCamera = std::make_shared<Camera>();
    FVEC3 vEye = FVEC3(1.0f, 1.0f, 0.0f);
    FVEC3 vAt = FVEC3(-1.0f, -1.0f, 0.0f);
    Camera::FrustumVolume tFrust;
    tFrust.fAspect = 1080.0f / 800.f;
    tFrust.fFovY = 90.0f;
    tFrust.nFarPlane = 1000.0f;
    tFrust.nNearPlane = 1.0f;

    Camera::ViewVolume tView;
    tView.nPosX = 0;
    tView.nPoxY = 0;
    tView.nWidht = 1080;
    tView.nHeight = 800;

    m_spCamera->Init(vEye, vAt);
    m_spCamera->Init(tFrust, tView);

    Triangle();

    bool running = true;
    while (running)
    {
        MSG msg;
        while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) 
        {
            if (msg.message == WM_QUIT) 
            {
                running = false;
            }
            else 
            {
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }
        }

        Frame(m_spProgram->GetProgramID(), m_spCamera);

        SwapBuffers(m_spWindow->GetDeviceContext());
    }

    return 0;
}
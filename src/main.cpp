#include <windows.h>
#include "glad/glad.h"
#include "glad/glad_wgl.h"
#include <iostream>

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
        -0.5f, -0.5f, 0.0f, 5.0f, // left  
         0.5f, -0.5f, 0.0f, 5.0f, // right 
         0.0f,  0.5f, 0.0f, 5.0f // top   
    }; 

    float colors[] = {
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
    };

    unsigned short indices[] = {
        0, 1, 2
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)sizeof(colors));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); 
}

void Frame(const UINT64 _programID)
{
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // glGetUniformLocation()
    // glUniformMatrix4fv()
    glUseProgram(_programID);
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, nullptr);
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

        Frame(m_spProgram->GetProgramID());

        SwapBuffers(m_spWindow->GetDeviceContext());
    }

    return 0;
}
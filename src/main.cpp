#include <windows.h>
#include <glad/gl.h>
#include <glad/wgl.h>
#include <iostream>

#pragma comment(linker, "/entry:WinMainCRTStartup")
#pragma comment(linker, "/subsystem:console")

// Updated and simplified from nickrolfe's gist: https://gist.github.com/nickrolfe/1127313ed1dbf80254b614a721b3ee9c


static LRESULT CALLBACK WindowCallbackFn(HWND window, UINT msg, WPARAM wparam, LPARAM lparam)
{
    LRESULT res = 0;

    switch (msg) 
    {
        case WM_CLOSE:
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            res = DefWindowProcA(window, msg, wparam, lparam);
            break;
    }

    return res;
}

static HWND CreateWindowInstance(HINSTANCE _inst, int _width, int _height)
{
    WNDCLASSA windowClass = { 0 };
    windowClass.style           = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    windowClass.lpfnWndProc     = WindowCallbackFn;
    windowClass.hInstance       = _inst;
    windowClass.hCursor         = LoadCursor(0, IDC_ARROW);
    windowClass.hbrBackground   = 0;
    windowClass.lpszClassName   = "GLWindow";

    if (!RegisterClassA(&windowClass))
    {
        DWORD errCode = GetLastError();
        std::cout << "Failed to register window. Err:" << errCode << "\n" ;
    }

    RECT rect;
    rect.left   = 0;
    rect.top    = 0;
    rect.right  = _width;
    rect.bottom = _height;
 
    DWORD windowStyle = WS_OVERLAPPEDWINDOW;
    AdjustWindowRect(&rect, windowStyle, false);

    HWND window = CreateWindowExA(
        0,
        windowClass.lpszClassName,
        "OpenGL",
        windowStyle,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        0,
        0,
        _inst,
        0);

    if (!window) 
    {
        std::cout << "Failed to create window.\n";
    }

    return window;
}

HGLRC InitOpenGL(HWND _window)
{
    HDC winHDC = GetDC(_window);

    PIXELFORMATDESCRIPTOR windowPixelFormatDesc = { 0 };
    windowPixelFormatDesc.nSize         = sizeof(windowPixelFormatDesc);
    windowPixelFormatDesc.nVersion      = 1;
    windowPixelFormatDesc.iPixelType    = PFD_TYPE_RGBA;
    windowPixelFormatDesc.dwFlags       = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    windowPixelFormatDesc.cColorBits    = 32;
    windowPixelFormatDesc.cAlphaBits    = 8;
    windowPixelFormatDesc.iLayerType    = PFD_MAIN_PLANE;
    windowPixelFormatDesc.cDepthBits    = 24;
    windowPixelFormatDesc.cStencilBits  = 8;

    int pixelFormat = ChoosePixelFormat(winHDC, &windowPixelFormatDesc);
    if (!pixelFormat)
    {
        std::cout << "Unable to find a suitable pixel format for the requested description.\n";
        return (HGLRC)0;
    }
    
    if (!SetPixelFormat(winHDC, pixelFormat, &windowPixelFormatDesc)) 
    {
        std::cout << "Unable to set the pixel format.\n";
    }

    HGLRC glContext = wglCreateContext(winHDC);
    if (!glContext)
    {
        std::cout << "Unable to create OpenGL context.\n";
    }

    if (!wglMakeCurrent(winHDC, glContext)) {
        std::cout << "Unable to apply OpenGL context to window.\n";

    }

    if (!gladLoaderLoadGL()) 
    {
        std::cout << " Failed to initialize GLAD" << std::endl;
    }

    return glContext;
}

void PrintGLVersionInformation()
{
    std::cout<< "GL_VENDOR: "       << glGetString(GL_VENDOR)       << "\n";
    std::cout<< "GL_RENDERER: "     << glGetString(GL_RENDERER)     << "\n";
    std::cout<< "GL_VERSION: "      << glGetString(GL_VERSION)      << "\n";
    // Uncomment for long string...
    // std::cout<< "GL_EXTENSIONS: "   << glGetString(GL_EXTENSIONS)   << "\n";
}

int shaderProgram = 0;
unsigned int VBO, VAO;

void Triangle()
{
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;

    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
    }; 

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 
}

void Frame()
{
    static float Color = 0.0f;
    Color = Color >= 1.0f ? 0.0f : Color + 0.001f;
    glClearColor( Color, 1.0f - Color, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	HINSTANCE   hInst           = GetModuleHandle(0);
    HWND        windowInstance  = CreateWindowInstance(hInst, 800, 600);
    HDC         winHDC          = GetDC(windowInstance);
    HGLRC       glContext       = InitOpenGL(windowInstance);


    PrintGLVersionInformation();

    ShowWindow(windowInstance, SW_SHOW);
    UpdateWindow(windowInstance);

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

        Frame();

        SwapBuffers(winHDC);
    }

    return 0;
}
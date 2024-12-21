#include "NewWindow.h"
#include <iostream>

NewWindow::NewWindow()
{}

NewWindow::~NewWindow()
{}

bool NewWindow::Create(const UINT32 _width, const UINT32 _height)
{
    HINSTANCE hInstance = GetModuleHandle(0);

    m_WindowClass ={0};

    m_WindowClass.style           = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    m_WindowClass.lpfnWndProc     = NewWindow::InputCallback;
    m_WindowClass.hInstance       = hInstance;
    m_WindowClass.hCursor         = LoadCursor(0, IDC_ARROW);
    m_WindowClass.hbrBackground   = 0;
    m_WindowClass.lpszClassName   = "WGL Window";

    if (!RegisterClassA(&m_WindowClass))
    {
        DWORD errCode = GetLastError();
        std::cout << "[WGLWindow] Failed to register window. Err:" << errCode << "\n" ;

        return false;
    }

    RECT rect;
    rect.left   = 0;
    rect.top    = 0;
    rect.right  = _width;
    rect.bottom = _height;
 
    DWORD windowStyle = WS_OVERLAPPEDWINDOW;
    AdjustWindowRect(&rect, windowStyle, false);

    m_WindowInstance = CreateWindowExA(
        0,
        m_WindowClass.lpszClassName,
        "OpenGL",
        windowStyle,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        0,
        0,
        hInstance,
        0);
        
    if (!m_WindowInstance) 
    {
        std::cout << "Failed to create window.\n";

        return false;
    }

    m_DeviceContext = GetDC(m_WindowInstance);
    if (!m_DeviceContext)
    {
        std::cout << "[WGLWindow] Failed to create device context";

        return false;
    }

    return true;
}

const HWND NewWindow::GetWindowInstance()
{
    return m_WindowInstance;
}

const HDC NewWindow::GetDeviceContext()
{
    return m_DeviceContext;
}

LRESULT CALLBACK NewWindow::InputCallback(HWND _windowInstance, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
    LRESULT result = 0;

    switch (_message) 
    {
        case WM_CLOSE:
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            result = DefWindowProcA(_windowInstance, _message, _wParam, _lParam);
            break;
    }

    return result;
}
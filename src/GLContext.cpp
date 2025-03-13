#include "GLContext.h"
#include "glad/glad_wgl.h"

#include <LogProject/Log.h>

GLContext::GLContext()
    : RenderContext()
{}

GLContext::~GLContext()
{
	if (!m_RenderingContext)
	{
		wglDeleteContext(m_RenderingContext);
	}
}

bool GLContext::Init(const HWND _windowInstance)
{
    m_DeviceContext = GetDC(_windowInstance);

	if (!InitPixel(m_DeviceContext))
	{
		LOGERROR() << "[GLContext] Failed to init pixel format";
		return false;
	}

	if (!InitContext(m_DeviceContext))
	{
		LOGERROR() << "[GLContext] Failed to init rendering context";
		return false;
	}

    if (!MakeCurrent())
    {
        return false;
    }

	if (!gladLoadGL())
	{
		LOGERROR() << "[GLContext] Failed to load gl function";
		return false;
	}

    LOGINFO() << "[GLContext] Successfully initialized OpenGL context";

    return true;
}

bool GLContext::MakeCurrent()
{
    if (!wglMakeCurrent(m_DeviceContext, m_RenderingContext)) 
    {
        LOGERROR() << "Unable to apply OpenGL context to window";

        return false;
    }

	return true;
}

bool GLContext::InitPixel(const HDC _deviceContext)
{
    if (!_deviceContext)
    {
        LOGERROR() << "[GLContext] The device context is invalid, failed to init wgl pixel format";

        return false;
    }

    PIXELFORMATDESCRIPTOR windowPixelFormatDesc;
    ZeroMemory(&windowPixelFormatDesc, sizeof(PIXELFORMATDESCRIPTOR));

    windowPixelFormatDesc.nSize         = sizeof(windowPixelFormatDesc);
    windowPixelFormatDesc.nVersion      = 1;
    windowPixelFormatDesc.iPixelType    = PFD_TYPE_RGBA;
    windowPixelFormatDesc.dwFlags       = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    windowPixelFormatDesc.cColorBits    = 32;
    windowPixelFormatDesc.cAlphaBits    = 8;
    windowPixelFormatDesc.iLayerType    = PFD_MAIN_PLANE;
    windowPixelFormatDesc.cDepthBits    = 24;
    windowPixelFormatDesc.cStencilBits  = 8;

    int pixelFormat = ChoosePixelFormat(_deviceContext, &windowPixelFormatDesc);
    if (!pixelFormat)
    {
        LOGERROR() << "[GLContext] Unable to find a suitable pixel format for the requested description";
        
        return false;
    }
    
    if (!SetPixelFormat(_deviceContext, pixelFormat, &windowPixelFormatDesc)) 
    {
        LOGERROR() << "[GLContext] Unable to set the pixel format";

        return false;
    }

    LOGINFO() << "[GLContext] Successfully set pixel format";

	return true;
}

bool GLContext::InitContext(const HDC _deviceContext)
{
    if (!_deviceContext)
    {
        LOGERROR() << "[GLContext] The device context is invalid, failed to init wgl rendering context";

        return false;
    }

	int attribList[5] = 
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3, 
		0
	};

	HGLRC tempContext = wglCreateContext(_deviceContext);
	if (!tempContext) 
	{
		LOGERROR() << "[GLContext] Failed to create temporary context";
		return false;
	}

	if (!wglMakeCurrent(_deviceContext, tempContext)) 
	{
		LOGERROR() << "[GLContext] Failed to activate temporary context";
		return false;
	}

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	if (!wglCreateContextAttribsARB) 
	{
		LOGERROR() << "[GLContext] Failed to load wglCreateContextAttribsARB";
		wglDeleteContext(tempContext);
		return false;
	}

    m_RenderingContext = wglCreateContextAttribsARB(_deviceContext, 0, attribList);
    if (!m_RenderingContext)
    {
        LOGERROR() << "Unable to create OpenGL context";
        return false;
    }

    LOGINFO() << "[GLContext] Successfully created OpenGL context";

	return true;
}
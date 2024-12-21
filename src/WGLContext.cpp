#include "WGLContext.h"
#include "glad/glad_wgl.h"
#include "NewLog.h"

WGLContext::WGLContext()
    : IContext()
{}

WGLContext::~WGLContext()
{
	if (!m_RenderingContext)
	{
		wglDeleteContext(m_RenderingContext);
	}
}

bool WGLContext::Init(const HDC _deviceContext)
{
	if (!InitPixel(_deviceContext))
	{
		LOG(ERROR) << "Failed to init pixel format";
		return false;
	}

	if (!InitContext(_deviceContext))
	{
		LOG(ERROR) << "Failed to init rendering context";
		return false;
	}

    if (!MakeCurrent(_deviceContext))
    {
        return false;
    }

	if (!gladLoadGL())
	{
		LOG(ERROR) << "Failed to load gl function";
		return false;
	}

    return true;
}

bool WGLContext::MakeCurrent(const HDC _deviceContext)
{
    if (!wglMakeCurrent(_deviceContext, m_RenderingContext)) 
    {
        LOG(ERROR) << "Unable to apply OpenGL context to window.\n";

        return false;
    }

	return true;
}

const HGLRC WGLContext::GetRenderingContext()
{
    return m_RenderingContext;
}

bool WGLContext::InitPixel(const HDC _deviceContext)
{
    if (!_deviceContext)
    {
        LOG(ERROR) << "[WGLContext] The device context is invalid, failed to init wgl pixel format \n";

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
        LOG(ERROR) << "[WGLContext] Unable to find a suitable pixel format for the requested description.\n";
        
        return false;
    }
    
    if (!SetPixelFormat(_deviceContext, pixelFormat, &windowPixelFormatDesc)) 
    {
        LOG(ERROR) << "Unable to set the pixel format.\n";

        return false;
    }

	return true;
}

bool WGLContext::InitContext(const HDC _deviceContext)
{
    if (!_deviceContext)
    {
        LOG(ERROR) << "[WGLContext] The device context is invalid, failed to init wgl rendering context";

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
		LOG(ERROR) << "Failed to create temporary context \n";
		return false;
	}

	if (!wglMakeCurrent(_deviceContext, tempContext)) 
	{
		LOG(ERROR) << "Failed to activate temporary context \n";
		return false;
	}

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	if (!wglCreateContextAttribsARB) 
	{
		LOG(ERROR) << "Failed to load wglCreateContextAttribsARB \n";
		wglDeleteContext(tempContext);
		return false;
	}

    m_RenderingContext = wglCreateContextAttribsARB(_deviceContext, 0, attribList);
    if (!m_RenderingContext)
    {
        LOG(ERROR) << "Unable to create OpenGL context.\n";
        return false;
    }

	return true;
}

void WGLContext::PrintInfo()
{
    LOG(INFO) << "GL_VENDOR: " << glGetString(GL_VENDOR) << "\n";
    LOG(INFO) << "GL_RENDERER: " << glGetString(GL_RENDERER) << "\n";
    LOG(INFO) << "GL_VERSION: " << glGetString(GL_VERSION) << "\n";
}
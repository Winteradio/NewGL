#include "WindowContext.h"

#include <LogProject/Log.h>

WindowContext::WindowContext()
	: m_WindowClass{0}
{}

WindowContext::~WindowContext()
{}

bool WindowContext::Init(const UINT32 _width, const UINT32 _height)
{
	HINSTANCE hInstance = GetModuleHandle(0);

	m_WindowClass.style           = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	m_WindowClass.hInstance       = hInstance;
	m_WindowClass.hCursor         = LoadCursor(0, IDC_ARROW);
	m_WindowClass.hbrBackground   = 0;
	m_WindowClass.lpszClassName   = "WGL Window";
	m_WindowClass.lpfnWndProc     = WindowContext::InputCallback;

	if (!RegisterClassA(&m_WindowClass))
	{
		DWORD errCode = GetLastError();
		LOGERROR() << "[Window] Failed to register window | Error : " << static_cast<UINT64>(errCode);

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
		DWORD errCode = GetLastError();
		LOGERROR() << "[Window] Failed to create window instance | Error : " << static_cast<UINT64>(errCode);

		return false;
	}

	LOGINFO() << "[Window] Succssed to create window";

	return true;
}

const HWND WindowContext::GetWindowInstance()
{
	return m_WindowInstance;
}

LRESULT CALLBACK WindowContext::InputCallback(HWND _windowInstance, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	LRESULT result = 0;

	switch (_message) 
	{
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_MOUSEHOVER:
		case WM_MOUSELEAVE:
		{
			s_pInputFunc(_windowInstance, _message, _wParam, _lParam);
			break;
		}
		
		case WM_CLOSE:
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}

		default:
		{
			result = DefWindowProcA(_windowInstance, _message, _wParam, _lParam);
			break;
		}
	}

	return result;
}

void WindowContext::SetInputFunc(INPUTFUNC _pInputFunc)
{
	s_pInputFunc = _pInputFunc;
}

WindowContext::INPUTFUNC WindowContext::s_pInputFunc = DefWindowProcA;
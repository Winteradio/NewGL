#ifndef __WINDOWCONTEXT_H__
#define __WINDOWCONTEXT_H__

#include <windows.h>
#include "Type.h"

#pragma comment(linker, "/entry:WinMainCRTStartup")
#pragma comment(linker, "/subsystem:console")

class WindowContext
{
	public :
		using INPUTFUNC = LRESULT (*)(HWND _windowInstance, UINT _message, WPARAM _wParam, LPARAM _lParam);

		WindowContext();
		~WindowContext();

	public :
		bool		Init(const UINT32 _width, const UINT32 _height);	
		const HWND	GetWindowInstance();

	public :
		static LRESULT CALLBACK InputCallback(HWND _windowInstance, UINT _message, WPARAM _wParam, LPARAM _lParam);
		static void SetInputFunc(INPUTFUNC _pInputFunc);

	private :
		static INPUTFUNC s_pInputFunc;

	private :
		HWND		m_WindowInstance;
		WNDCLASSA   m_WindowClass;
};

#endif // __WINDOWCONTEXT_H__
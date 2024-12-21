#ifndef __NEWWINDOW_H__
#define __NEWWINDOW_H__

#include <windows.h>
#include "Type.h"

#pragma comment(linker, "/entry:WinMainCRTStartup")
#pragma comment(linker, "/subsystem:console")

class NewWindow
{
    public :
        NewWindow();
        ~NewWindow();

    public :
        bool		Create(const UINT32 _width, const UINT32 _height);
        const HWND	GetWindowInstance();
        const HDC	GetDeviceContext();

	public :
		static LRESULT CALLBACK InputCallback(HWND _windowInstance, UINT _message, WPARAM _wParam, LPARAM _lParam);

    private :
        HWND		m_WindowInstance;
        HDC			m_DeviceContext;
        WNDCLASSA   m_WindowClass;
};

#endif // __NEWWINDOW_H__
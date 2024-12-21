#ifndef __ICONTEXT_H__
#define __ICONTEXT_H__

#include <windows.h>

class IContext
{
	public :
		IContext() = default;
		virtual ~IContext() = default;

	public :
		virtual bool Init(const HDC _deviceContext) = 0;
		virtual bool MakeCurrent(const HDC _deviceContext) = 0;

	private :
		virtual void PrintInfo() = 0;
};

#endif // __ICONTEXT_H__
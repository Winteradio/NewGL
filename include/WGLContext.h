#ifndef __WGLCONTEXT_H__
#define __WGLCONTEXT_H__

#include "IContext.h"

class WGLContext : public IContext
{
	public :
		WGLContext();
		virtual ~WGLContext();

	public :
		bool Init(const HDC _deviceContext) final;
		bool MakeCurrent(const HDC _deviceContext) final;
		const HGLRC GetRenderingContext();

	private :
		bool InitPixel(const HDC _deviceContext);
		bool InitContext(const HDC _deviceContext);

		void PrintInfo() final;

	private :
		HDC m_DeviceContext;
		HGLRC m_RenderingContext;
};

#endif // __WGLCONTEXT_H__
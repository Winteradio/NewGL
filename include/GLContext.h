#ifndef __GLCONTEXT_H__
#define __GLCONTEXT_H__

#include "RenderContext.h"

class GLContext : public RenderContext
{
	public :
		GLContext();
		virtual ~GLContext();

	public :
		bool Init(const HWND _windowInstance) final;
		void Prepare(const ViewState* _renderState) final {};
		void Draw(const IRenderResource* _renderResource) final {};
		void Postpare() final {};
		const HDC GetDeviceContext() final
		{
			return m_DeviceContext;
		}

	public :
		bool InitPixel(const HDC _deviceContext);
		bool InitContext(const HDC _deviceContext);

		bool MakeCurrent();

	private :
		HDC m_DeviceContext;
		HGLRC m_RenderingContext;
};

#endif // __GLCONTEXT_H__
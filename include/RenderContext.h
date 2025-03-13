#ifndef __RENDERCONTEXT_H__
#define __RENDERCONTEXT_H__

#include <windows.h>
#include <memory>

class IRenderResource;
class ViewState;

class RenderContext
{
	public :
		RenderContext() = default;
		virtual ~RenderContext() = default;

	public :
		virtual bool Init(const HWND _windowInstance) = 0;
		virtual void Prepare(const ViewState* _renderState) = 0;
		virtual void Draw(const IRenderResource* _renderResource) = 0;
		virtual void Postpare() = 0;

		virtual const HDC GetDeviceContext() = 0;
};

#endif // __RENDERCONTEXT_H__
#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <memory>
#include "GraphicType.h"

class Camera;
class Program;
class WindowContext;
class GLContext;

class Engine
{
    public :
        Engine();
        ~Engine();

    public :
        bool Init(const UINT32 _width, const UINT32 _height);
        void Update();
        void Draw();

        void Add(const Graphic::Geometry& _geometry);

    private :
        std::shared_ptr<WindowContext> m_spWindow;
        std::shared_ptr<GLContext> m_spContext;
        std::shared_ptr<Program> m_spProgram;
        std::shared_ptr<Camera> m_spCamera;

        std::vector<Graphic::Mesh> m_vMeshes;
};

#endif // __ENGINE_H__
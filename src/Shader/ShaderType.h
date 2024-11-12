#ifndef __SHADERTYPE_H__
#define __SHADERTYUP_H__

enum GraphicType
{
    None = 0,
    OpenGL = 1,
    DirectX = 2,
    Vulkan = 3,
    Metal = 4,
};

enum ShaderType
{
    None = 0,
    Vertex = 1,
    Geometry = 2,
    Hull = 3,
    Fragment = 4,
    None = 5,
};

#endif // __SHADERTYPE_H__
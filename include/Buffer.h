#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "Type.h"

struct Component
{
    enum class ElementType : uint8
    {
        NONE = 0,
        INT8,
        INT16,
        INT32,
        INT64,

        UINT8,
        UINT16,
        UINT32,
        UINT64,
        
        FLOAT,
        DOUBLE
    };

    ElementType Type;
    uint8 Size;
    uint16 Count;

    Component();
    Component(const ElementType _type, const uint8 _size, const uint16 _count);
};

struct Buffer
{
    uint32 VAO;
    uint32 VBO;
    uint32 EBO;

    Component component;

    Buffer() = delete;
    Buffer(const uint32 _vao, const uint32 _vbo, const uint32 _ebo);
};

#endif // __BUFFER_H__
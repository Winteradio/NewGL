#ifndef __GRAPHIC_TYPE_H__
#define __GRAPHIC_TYPE_H__

#include "Type.h"
#include "GeoType.h"
#include <vector>

namespace Graphic
{
	enum class eDrawMode : UINT8
	{
		NONE = 0,
		POINTS,
		LINES,
		LINE_LOOP,
		LINE_STRIP,
		TRIANGLES,
		TRIANGLE_STRIP,
		TRIANGLE_FAN,
		QUADS,
		QUAD_STRIP,
		POLYGON
	};

	enum class ePolygonMode : UINT8
	{
		NONE = 0,
		POINT,
		LINE,
		FILL
	};

	enum class eBufferType : UINT8
	{
		NONE = 0,
		VERTEX,
		INDEX
	};

	enum class eComponentType : UINT8
	{
		NONE = 0,
		BYTE,
		UNSIGNED_BYTE,
		SHORT,
		UNSIGNED_SHORT,
		INT,
		UNSIGNED_INT,
		FLOAT,
		DOUBLE
	};

	struct Buffer
	{
		UINT32 ID;
		UINT32 Target;
		UINT64 Offset;
	
		UINT64 Size;
		UINT64 Type;
		UINT64 Count;
		UINT8 Dimension;
	
		Buffer()
			: ID(0)
			, Target(0)
			, Offset(0)
			, Size(0)
			, Type(0)
			, Count(0)
			, Dimension(0)
		{}
	};
	
	struct Mesh
	{
		Buffer VertexBuffer;
		Buffer IndexBuffer;
	
		UINT32 ID;
		UINT64 DrawMode;
		UINT64 PolygonMode;
	
		Mesh()
			: VertexBuffer()
			, IndexBuffer()
			, ID(0)
			, DrawMode(0)
			, PolygonMode(0)
		{}
	};
	
	struct Geometry
	{
		std::vector<FVEC3> Vertices;
		std::vector<UINT32> Indices;
	
		UINT64 DrawMode;
		UINT64 PolygonMode;
	
		Geometry()
			: Vertices()
			, Indices()
			, DrawMode(0)
			, PolygonMode(0)
		{}	
	};	
};

#endif // __GRAPHIC_TYPE_H__
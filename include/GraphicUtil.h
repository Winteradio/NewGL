#ifndef __GRAPHIC_UTIL_H__
#define __GRAPHIC_UTIL_H__

#include "GraphicType.h"

namespace Graphic
{
	namespace Util
	{
		Mesh Create(const Geometry& _geometry);		
		Geometry Create(const std::vector<FVEC3>& _vertices, const std::vector<UINT32>& _indices, const eDrawMode _drawMode = eDrawMode::NONE, const ePolygonMode _polygonMode = ePolygonMode::NONE);
		Geometry Create(const std::vector<FVEC3>& _vertices, const eDrawMode _drawMode = eDrawMode::NONE, const ePolygonMode _polygonMode = ePolygonMode::NONE);

		Geometry Create(const std::vector<FVEC2>& _vertices, const std::vector<UINT32>& _indices, const eDrawMode _drawMode = eDrawMode::NONE, const ePolygonMode _polygonMode = ePolygonMode::NONE);
		Geometry Create(const std::vector<FVEC2>& _vertices, const eDrawMode _drawMode = eDrawMode::NONE, const ePolygonMode _polygonMode = ePolygonMode::NONE);

		bool Generate(const Geometry& _geometry, Mesh& _mesh);
		void Delete(Mesh& _mesh);
	};
};

#endif // __GRAPHIC_UTIL_H__
#include "GraphicUtil.h"

#include <LogProject/Log.h>

#include "glad/glad.h"
#include "glad/glad_wgl.h"

namespace Graphic
{
	namespace Internal
	{
		const UINT64 GetDrawMode(const eDrawMode _drawMode)
		{
			switch (_drawMode)
			{
				case eDrawMode::POINTS:
					return GL_POINTS;
				case eDrawMode::LINES:
					return GL_LINES;
				case eDrawMode::LINE_LOOP:
					return GL_LINE_LOOP;
				case eDrawMode::LINE_STRIP:
					return GL_LINE_STRIP;
				case eDrawMode::TRIANGLES:
					return GL_TRIANGLES;
				case eDrawMode::TRIANGLE_STRIP:
					return GL_TRIANGLE_STRIP;
				case eDrawMode::TRIANGLE_FAN:
					return GL_TRIANGLE_FAN;
				case eDrawMode::QUADS:
					return GL_QUADS;
				case eDrawMode::QUAD_STRIP:
				case eDrawMode::POLYGON:
				default:
					return GL_NONE;
			}
		}

		const UINT64 GetPolygonMode(const ePolygonMode _polygonMode)
		{
			switch (_polygonMode)
			{
				case ePolygonMode::POINT:
					return GL_POINT;
				case ePolygonMode::LINE:
					return GL_LINE;
				case ePolygonMode::FILL:
					return GL_FILL;
				default:
					return GL_NONE;
			}
		}

		const UINT64 GetBufferTarget(const eBufferType _bufferType)
		{
			switch (_bufferType)
			{
				case eBufferType::VERTEX:
					return GL_ARRAY_BUFFER;
				case eBufferType::INDEX:
					return GL_ELEMENT_ARRAY_BUFFER;
				default:
					return GL_NONE;
			}
		}

		template<typename T>
		struct ComponentDimension 
		{
			static constexpr UINT8 Dimension = 1;
		};

		template<glm::length_t L, typename T, glm::qualifier Q>
		struct ComponentDimension<glm::vec<L, T, Q>> 
		{
			static constexpr UINT8 Dimension = L;
		};

		template<typename T>
		struct ComponentDimension<std::vector<T>> 
		{
			static constexpr UINT8 Dimension = ComponentDimension<T>::Dimension;
		};

		template<typename T>
		constexpr UINT8 GetComponentDimension(const T& _value) 
		{
			return ComponentDimension<T>::Dimension;
		}

		template<typename T>
		struct ComponentType {
			using Type = T;
		};

		template<glm::length_t L, typename T, glm::qualifier Q>
		struct ComponentType<glm::vec<L, T, Q>> 
		{
			using Type = T;
		};

		template<typename T>
		struct ComponentType<std::vector<T>> 
		{
			using Type = typename ComponentType<T>::Type;
		};

		template<typename T, typename U>
		struct IsSame
		{
			static constexpr bool Value = false;
		};

		template<typename T>
		struct IsSame<T, T> 
		{
			static constexpr bool Value = true;
		};

		template<typename T>
		const UINT64 GetComponentType(const T& _value)
		{
			using Type = typename ComponentType<T>::Type;

			if (IsSame<Type, INT8>::Value)
			{
				return GL_BYTE;
			}
			else if (IsSame<Type, UINT8>::Value)
			{
				return GL_UNSIGNED_BYTE;
			}
			else if (IsSame<Type, INT16>::Value)
			{
				return GL_SHORT;
			}
			else if (IsSame<Type, UINT16>::Value)
			{
				return GL_UNSIGNED_SHORT;
			}
			else if (IsSame<Type, INT32>::Value)
			{
				return GL_INT;
			}
			else if (IsSame<Type, UINT32>::Value)
			{
				return GL_UNSIGNED_INT;
			}
			else if (IsSame<Type, FLOAT>::Value || IsSame<Type, FLOAT32>::Value)
			{
				return GL_FLOAT;
			}
			else if (IsSame<Type, FLOAT64>::Value)
			{
				return GL_DOUBLE;
			}
			else
			{
				return GL_NONE;
			}
		}

		void FitVertices(std::vector<FVEC3>& _vertices)
		{
			constexpr FLOAT32 maxRange = 0.5f;

			if (_vertices.empty())
			{
				return;
			}

			FVEC3 min = FVEC3(FLT_EPSILON, FLT_EPSILON, 0.0f);
			FVEC3 max = FVEC3(-FLT_EPSILON, -FLT_EPSILON, 0.0f);
			for (auto& vertex : _vertices)
			{
				min.x = std::min(min.x, vertex.x);
				min.y = std::min(min.y, vertex.y);

				max.x = std::max(max.x, vertex.x);
				max.y = std::max(max.y, vertex.y);
			}

			FVEC3 center = (min + max) / 2.0f;

			FLOAT32 scale = 2.0f * maxRange / std::max(max.x - min.x, max.y - min.y);
			for (auto& vertex : _vertices)
			{
				vertex = (vertex-center) * scale;
			}
		}
	}

	namespace Util
	{
		Mesh Create(const Geometry& _geometry)
		{
			Mesh mesh;
			mesh.DrawMode = _geometry.DrawMode;
			mesh.PolygonMode = _geometry.PolygonMode;

			mesh.VertexBuffer.Target = Internal::GetBufferTarget(eBufferType::VERTEX);
			mesh.VertexBuffer.Offset = 0;
			mesh.VertexBuffer.Size = sizeof(decltype(_geometry.Vertices)::value_type) * _geometry.Vertices.size();
			mesh.VertexBuffer.Type = Internal::GetComponentType(_geometry.Vertices);
			mesh.VertexBuffer.Dimension = Internal::GetComponentDimension(_geometry.Vertices);
			mesh.VertexBuffer.Count = _geometry.Vertices.size();

			mesh.IndexBuffer.Target = Internal::GetBufferTarget(eBufferType::INDEX);
			mesh.IndexBuffer.Offset = 0;
			mesh.IndexBuffer.Size = sizeof(decltype(_geometry.Indices)::value_type) * _geometry.Indices.size();
			mesh.IndexBuffer.Type = Internal::GetComponentType(_geometry.Indices);
			mesh.IndexBuffer.Dimension = Internal::GetComponentDimension(_geometry.Indices);
			mesh.IndexBuffer.Count = _geometry.Indices.size();

			return mesh;
		}

		Geometry Create(const std::vector<FVEC3>& _vertices, const std::vector<UINT32>& _indices, const eDrawMode _drawMode, const ePolygonMode _polygonMode)
		{
			Geometry geometry;
			geometry.Vertices = _vertices;
			geometry.Indices = _indices;
			geometry.DrawMode = Internal::GetDrawMode(_drawMode);
			geometry.PolygonMode = Internal::GetPolygonMode(_polygonMode);

			Internal::FitVertices(geometry.Vertices);

			return geometry;
		}

		Geometry Create(const std::vector<FVEC3>& _vertices, const eDrawMode _drawMode, const ePolygonMode _polygonMode)
		{
			Geometry geometry;
			geometry.Vertices = _vertices;
			geometry.DrawMode = Internal::GetDrawMode(_drawMode);
			geometry.PolygonMode = Internal::GetPolygonMode(_polygonMode);

			geometry.Indices.resize(_vertices.size());
			for (UINT32 i = 0; i < _vertices.size(); ++i)
			{
				geometry.Indices[i] = i;
			}

			Internal::FitVertices(geometry.Vertices);

			return geometry;
		}

		Geometry Create(const std::vector<FVEC2>& _vertices, const std::vector<UINT32>& _indices, const eDrawMode _drawMode, const ePolygonMode _polygonMode)
		{
			Geometry geometry;
			geometry.Indices = _indices;
			geometry.DrawMode = Internal::GetDrawMode(_drawMode);
			geometry.PolygonMode = Internal::GetPolygonMode(_polygonMode);

			geometry.Vertices = std::vector<FVEC3>(_vertices.size());
			for (UINT32 i = 0; i < _vertices.size(); ++i)
			{
				geometry.Vertices[i] = FVEC3(_vertices[i], 0.0f);
			}

			Internal::FitVertices(geometry.Vertices);

			return geometry;
		}

		Geometry Create(const std::vector<FVEC2>& _vertices, const eDrawMode _drawMode, const ePolygonMode _polygonMode)
		{
			Geometry geometry;
			geometry.DrawMode = Internal::GetDrawMode(_drawMode);
			geometry.PolygonMode = Internal::GetPolygonMode(_polygonMode);

			geometry.Vertices = std::vector<FVEC3>(_vertices.size());
			for (UINT32 i = 0; i < _vertices.size(); ++i)
			{
				geometry.Vertices[i] = FVEC3(_vertices[i], 0.0f);
			}

			geometry.Indices.resize(_vertices.size());
			for (UINT32 i = 0; i < _vertices.size(); ++i)
			{
				geometry.Indices[i] = i;
			}

			Internal::FitVertices(geometry.Vertices);

			return geometry;
		}

		bool Generate(const Geometry& _geometry, Mesh& _mesh)
		{
			glGenVertexArrays(1, &_mesh.ID);
			glGenBuffers(1, &_mesh.VertexBuffer.ID);
			glGenBuffers(1, &_mesh.IndexBuffer.ID);

			if (_mesh.ID == GL_NONE || _mesh.VertexBuffer.ID == GL_NONE || _mesh.IndexBuffer.ID == GL_NONE)
			{
				LOGERROR() << "[Graphic::Util] Failed to generate Mesh buffer";
				return false;
			}

			glBindVertexArray(_mesh.ID);

			glBindBuffer(_mesh.VertexBuffer.Target, _mesh.VertexBuffer.ID);
			glBufferData(_mesh.VertexBuffer.Target, _mesh.VertexBuffer.Size, _geometry.Vertices.data(), GL_STATIC_DRAW);
		
			glBindBuffer(_mesh.IndexBuffer.Target, _mesh.IndexBuffer.ID);
			glBufferData(_mesh.IndexBuffer.Target, _mesh.IndexBuffer.Size, _geometry.Indices.data(), GL_STATIC_DRAW);
		
			glVertexAttribPointer(0, _mesh.VertexBuffer.Dimension, _mesh.VertexBuffer.Type, GL_FALSE, 0, nullptr);
			glEnableVertexAttribArray(0);
		
			glBindVertexArray(0);

			return true;
		}

		void Delete(Mesh& _mesh)
		{
			if (_mesh.ID != GL_NONE)
			{
				glBindVertexArray(GL_NONE);
				glDeleteVertexArrays(1, &_mesh.ID);
				_mesh.ID = GL_NONE;
			}

			if (_mesh.VertexBuffer.ID != GL_NONE)
			{
				glBindBuffer(_mesh.VertexBuffer.Target, GL_NONE);
				glDeleteBuffers(1, &_mesh.VertexBuffer.ID);
				_mesh.VertexBuffer.ID = GL_NONE;
			}

			if (_mesh.IndexBuffer.ID != GL_NONE)
			{
				glBindBuffer(_mesh.IndexBuffer.Target, GL_NONE);
				glDeleteBuffers(1, &_mesh.IndexBuffer.ID);
				_mesh.IndexBuffer.ID = GL_NONE;
			}
		}
	}
}
#pragma once
#include <glad/glad.h>
#include <numeric>

#include "shader.hpp"

namespace tiny
{
	using pipeline_handle = uint32_t;

	// Data Type:
	enum class DataType
	{
		BYTE = 0,
		UNSIGNED_BYTE,
		SHORT,
		UNSIGNED_SHORT,
		INT,
		UNSIGNED_INT,
		FLOAT,
		DOUBLE,
	};

	inline constexpr uint32_t getDataTypeGL(DataType type)
	{
		switch (type)
		{
			case DataType::BYTE: 
				return GL_BYTE;

			case DataType::UNSIGNED_BYTE: 
				return GL_UNSIGNED_BYTE;

			case DataType::SHORT: 
				return GL_SHORT;

			case DataType::UNSIGNED_SHORT: 
				return GL_UNSIGNED_SHORT;

			case DataType::INT: 
				return GL_INT;

			case DataType::UNSIGNED_INT: 
				return GL_UNSIGNED_INT;

			case DataType::FLOAT: 
				return GL_FLOAT;

			case DataType::DOUBLE: 
				return GL_DOUBLE;
		}

		return NULL;
	}

	inline constexpr uint32_t getDataTypeSize(DataType type)
	{
		switch (type)
		{
			case DataType::BYTE:
				return sizeof(char);

			case DataType::UNSIGNED_BYTE:
				return sizeof(unsigned char);

			case DataType::SHORT:
				return sizeof(short);

			case DataType::UNSIGNED_SHORT:
				return sizeof(unsigned short);

			case DataType::INT:
				return sizeof(int);

			case DataType::UNSIGNED_INT:
				return sizeof(unsigned int);

			case DataType::FLOAT:
				return sizeof(float);

			case DataType::DOUBLE:
				return sizeof(double);
		}

		return 0;
	}

	// Primitive Topology:
	enum class PrimitiveTopology
	{
		POINTS,
		LINES,
		LINE_STRIP,
		TRIANGLES,
		TRIANGLE_STRIP,
	};

	inline constexpr uint32_t getTopologyGL(PrimitiveTopology topology)
	{
		switch (topology)
		{
			case PrimitiveTopology::POINTS: 
				return GL_POINTS;

			case PrimitiveTopology::LINES: 
				return GL_LINES;

			case PrimitiveTopology::LINE_STRIP:
				return GL_LINE_STRIP;

			case PrimitiveTopology::TRIANGLES: 
				return GL_TRIANGLES;

			case PrimitiveTopology::TRIANGLE_STRIP:
				return GL_TRIANGLE_STRIP;
		}

		return NULL;
	}

	// Vertex Layout:
	enum class InputRate
	{
		PER_VERTEX,
		PER_INSTANCE
	};

	struct VertexLayout
	{
	public:
		struct Element
		{
			uint32_t location;
			uint32_t dataAmount;
			DataType dataType;
			InputRate inputRate = InputRate::PER_VERTEX;
		};

		Element& add(const Element& element)
		{
			elements.push_back(element);
			return elements[elements.size() - 1];
		}

	public:
		std::vector<Element> elements;
	};

	// Polygon Mode:
	enum class PolygonMode
	{
		FILL,
		LINE,
		POINT,
		FILL_RECTANGLE
	};

	inline constexpr uint32_t getPolygonModeGL(PolygonMode polygonMode)
	{
		switch (polygonMode)
		{
			case PolygonMode::POINT:
				return GL_POINT;

			case PolygonMode::LINE:
				return GL_LINE;

			case PolygonMode::FILL:
				return GL_FILL;
		}

		return 0;
	}

	// Cull Mode:
	enum class CullMode : uint32_t
	{
		NONE = 1 << 0,
		FRONT = 1 << 1,
		BACK = 1 << 2,
		FRONT_AND_BACK = 1 << 3
	};

	inline constexpr uint32_t getCullModeGL(CullMode cullMode)
	{
		switch (cullMode)
		{
			case CullMode::NONE:
				return GL_NONE;

			case CullMode::FRONT:
				return GL_FRONT;

			case CullMode::BACK:
				return GL_BACK;

			case CullMode::FRONT_AND_BACK:
				return GL_FRONT_AND_BACK;
		}

		return 0;
	}

	// Front Face:
	enum class FrontFace
	{
		COUNTER_CLOCKWISE,
		CLOCKWISE
	};

	inline constexpr uint32_t getFrontFaceGL(FrontFace frontFace)
	{
		switch (frontFace)
		{
			case FrontFace::COUNTER_CLOCKWISE:
				return GL_CCW;

			case FrontFace::CLOCKWISE:
				return GL_CW;
		}

		return 0;
	}

	// Descriptor:
	struct PipelineDescriptor
	{
		VertexLayout vertexLayout;
		std::vector<shader_handle> shaderHandles;

		PrimitiveTopology primitiveTopology = PrimitiveTopology::TRIANGLES;
		PolygonMode polygonMode = PolygonMode::FILL;
		CullMode cullMode = CullMode::FRONT;
		FrontFace frontFace = FrontFace::COUNTER_CLOCKWISE;
	};

	inline void checkLinkingErrors(uint32_t programID)
	{
		int success;
		char infoLog[1024];

		glGetProgramiv(programID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(programID, 1024, NULL, infoLog);
			TINY_LOG_ERROR("Failed to link program: {}", infoLog);
		}
	}
}
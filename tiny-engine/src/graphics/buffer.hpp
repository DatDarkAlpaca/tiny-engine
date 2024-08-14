#pragma once
#include <numeric>

namespace tiny
{
	using buffer_handle = uint32_t;

	enum class BufferType
	{
		ARRAY_BUFFER,
		ELEMENT_ARRAY_BUFFER,
		UNIFORM_BUFFER
	};

	inline constexpr uint32_t getBufferTypeGL(BufferType type)
	{
		switch (type)
		{
			case BufferType::ARRAY_BUFFER:
				return GL_ARRAY_BUFFER;

			case BufferType::ELEMENT_ARRAY_BUFFER:
				return GL_ELEMENT_ARRAY_BUFFER;

			case BufferType::UNIFORM_BUFFER:
				return GL_UNIFORM_BUFFER;
		}
	}

	struct BufferDescriptor
	{
		BufferType type;
		size_t size;
		size_t usage = GL_STATIC_DRAW;
	};
}
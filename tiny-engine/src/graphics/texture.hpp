#pragma once
#include <numeric>

namespace tiny
{
	using texture_handle = uint32_t;

	enum class TextureFilter
	{
		NEAREST,
		LINEAR,
		NEAREST_MIPMAP_NEAREST,
		LINEAR_MIPMAP_NEAREST,
		NEAREST_MIPMAP_LINEAR
	};

	inline constexpr uint32_t getTextureFilterGL(TextureFilter filter)
	{
		switch (filter)
		{
			case TextureFilter::NEAREST: 
				return GL_NEAREST;

			case TextureFilter::LINEAR: 
				return GL_LINEAR;

			case TextureFilter::NEAREST_MIPMAP_NEAREST: 
				return GL_NEAREST_MIPMAP_NEAREST;

			case TextureFilter::LINEAR_MIPMAP_NEAREST: 
				return GL_LINEAR_MIPMAP_NEAREST;

			case TextureFilter::NEAREST_MIPMAP_LINEAR: 
				return GL_NEAREST_MIPMAP_LINEAR;
		}
	}

	enum TextureClamp
	{
		CLAMP_TO_EDGE,
		CLAMP_TO_BORDER, 
		MIRRORED_REPEAT, 
		REPEAT, 
		MIRROR_CLAMP_TO_EDGE
	};

	inline constexpr uint32_t getTextureClampGL(TextureClamp clamp)
	{
		switch (clamp)
		{
			case TextureClamp::CLAMP_TO_EDGE:
				return GL_CLAMP_TO_EDGE;

			case TextureClamp::CLAMP_TO_BORDER:
				return GL_CLAMP_TO_BORDER;

			case TextureClamp::MIRRORED_REPEAT:
				return GL_MIRRORED_REPEAT;

			case TextureClamp::REPEAT:
				return GL_REPEAT;

			case TextureClamp::MIRROR_CLAMP_TO_EDGE:
				return GL_MIRROR_CLAMP_TO_EDGE;
		}
	}

	struct TextureDescriptor
	{
		// linear: linear + linear
		// bilinear: LINEAR_MIPMAP_NEAREST + linear

		TextureFilter minFilter = TextureFilter::NEAREST, magFilter = TextureFilter::NEAREST;
		TextureClamp wrapS, wrapT;
		bool generateBipmap = false;
	};
}
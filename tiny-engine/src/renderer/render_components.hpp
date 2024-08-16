#pragma once
#include "graphics/texture.hpp"
#include "graphics/handle.hpp"

namespace tiny
{
	struct SpriteComponent
	{
	public:
		SpriteComponent() = default;

		SpriteComponent(texture_handle textureHandle)
			: textureHandle(textureHandle) { }

	public:
		texture_handle textureHandle = invalid_handle;
	};
}
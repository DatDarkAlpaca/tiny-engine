#pragma once
#include "core/buffer.hpp"
#include "assets.hpp"

namespace tiny
{
	struct TextureAsset
	{
		AssetMetadata metadata;
		Buffer buffer;

		uint32_t width = 0;
		uint32_t height = 0;
		uint32_t channels = 0;
	};
}
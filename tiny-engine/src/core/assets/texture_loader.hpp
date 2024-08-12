#pragma once
#include <filesystem>
#include "texture_asset.hpp"

namespace tiny
{
	class TextureLoader
	{
	public:
		static TextureAsset load(const std::filesystem::path& filepath);
	};
}
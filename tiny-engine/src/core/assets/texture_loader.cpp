#include "pch.hpp"
#include <stb_image.h>

#include "common.hpp"
#include "texture_loader.hpp"

namespace tiny
{
	TextureAsset TextureLoader::load(const std::filesystem::path& filepath)
	{
		if (!std::filesystem::exists(filepath))
		{
			TINY_LOG_ERROR("Failed to load texture: '{}'. File does not exist.", filepath.string());
			return {};
		}

		int width, height, channels;

		Buffer buffer;
		{
			buffer.data = stbi_load(filepath.string().c_str(), &width, &height, &channels, 4);
			buffer.size = width * height * channels;
		}

		TextureAsset asset;
		asset.metadata.filepath = filepath;

		asset.width = static_cast<uint32_t>(width);
		asset.height = static_cast<uint32_t>(height);
		asset.channels = static_cast<uint32_t>(channels);
		asset.buffer = buffer;

		return asset;
	}
}
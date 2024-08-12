#pragma once
#include "texture_asset.hpp"

namespace tiny
{
	class AssetLibrary
	{
	public:
		AssetHandle insertTexture(const TextureAsset& textureAsset)
		{
			m_Textures.push_back(std::make_shared<TextureAsset>(textureAsset));
			return AssetHandle(m_Textures.size() - 1);
		}

		std::weak_ptr<TextureAsset> getTexture(AssetHandle handle)
		{
			return m_Textures[static_cast<size_t>(handle)];
		}

	private:
		std::vector<std::shared_ptr<TextureAsset>> m_Textures;
	};
}
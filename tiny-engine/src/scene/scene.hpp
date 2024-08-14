#pragma once
#include <entt/entt.hpp>
#include <optional>

#include "utils/UUID.hpp"

namespace tiny
{
	class Entity;

	class Scene
	{
		friend class Entity;

	public:
		Scene() = default;

	public:
		Entity addEntity();
		Entity addEntity(const std::string& entityName);
		Entity addEntity(UUID uuid, const std::string& entityName);

		void removeEntity(UUID uuid);
		void removeEntity(Entity entity);

		std::optional<Entity> findEntityByUUID(UUID uuid) const;
		std::vector<Entity> findEntitiesByName(const std::string& name);

	public:
		inline uint32_t entityCount() const { return m_Diagnostics.entityCount; }

	private:
		entt::registry m_Registry;
		std::unordered_map<UUID, Entity> m_EntityMap;

		struct Diagnostics
		{
			uint32_t entityCount = 0;
		} m_Diagnostics;
	};
}
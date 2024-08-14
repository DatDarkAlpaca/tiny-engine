#include "pch.hpp"
#include "Scene.hpp"
#include "Entity.hpp"

#include "components.hpp"

namespace tiny
{
	Entity Scene::addEntity()
	{
		return addEntity({ }, { });
	}

	Entity Scene::addEntity(const std::string& entityName)
	{
		return addEntity({ }, entityName);
	}

	Entity Scene::addEntity(UUID uuid, const std::string& entityName)
	{
		Entity entity = { m_Registry.create(), this };

		// UUID:
		IDComponent& uuidComp = entity.AddComponent<IDComponent>(uuid);

		// Name:
		NameComponent& nameComp = entity.AddComponent<NameComponent>();
		{
			nameComp.name = entityName.empty() ? "Entity " + std::to_string(m_Diagnostics.entityCount) : entityName;
		}

		// Record:
		m_EntityMap[uuidComp.uuid] = entity;
		m_Diagnostics.entityCount++;

		return entity;
	}

	void Scene::removeEntity(UUID uuid)
	{
		m_EntityMap.erase(uuid);

		auto& entityOption = findEntityByUUID(uuid);
		if (!entityOption.has_value())
		{
			TINY_LOG_WARN("Failed to remove entity with UUID: {}", std::uint64_t(uuid));
			return;
		}

		m_Registry.destroy(entityOption.value());
	}

	void Scene::removeEntity(Entity entity)
	{
		m_EntityMap.erase(entity.UUID());
		m_Registry.destroy(entity);
	}

	std::optional<Entity> Scene::findEntityByUUID(UUID uuid) const
	{
		if (m_EntityMap.find(uuid) != m_EntityMap.end())
			return m_EntityMap.at(uuid);

		return std::nullopt;
	}

	std::vector<Entity> Scene::findEntitiesByName(const std::string& name)
	{
		std::vector<Entity> entities;
		for (auto [entity, nameComp] : m_Registry.view<NameComponent>().each())
		{
			if (nameComp.name == name)
				entities.push_back(Entity(entity, this));
		}

		return entities;
	}
}
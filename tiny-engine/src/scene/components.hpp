#pragma once
#include "utils/UUID.hpp"
#include <string>
#include <glm/glm.hpp>

namespace tiny
{
	struct IDComponent
	{
	public:
		IDComponent() = default;
		IDComponent(const IDComponent&) = default;

	public:
		UUID uuid;
	};

	struct NameComponent
	{
	public:
		NameComponent() = default;
		NameComponent(const NameComponent&) = default;
		NameComponent(const std::string& name) : name(name) { }

	public:
		std::string name;
	};

	struct TransformComponent
	{
	public:
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform) : transform(transform) { }

	public:
		glm::mat4 transform;
	};
}
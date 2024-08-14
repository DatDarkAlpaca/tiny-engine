#pragma once
#include "utils/UUID.hpp"
#include <string>

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
}
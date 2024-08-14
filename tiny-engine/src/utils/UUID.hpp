#pragma once
#include <numeric>
#include "utils/random.hpp"

namespace tiny
{
	class UUID
	{
	public:
		UUID()
			: m_UUID(generateRandom<std::uint64_t>())
		{

		}

	public:
		operator uint64_t() const { return m_UUID; }

	private:
		std::uint64_t m_UUID;
	};
}

namespace std 
{
	template <typename T> struct hash;

	template<>
	struct hash<tiny::UUID>
	{
		size_t operator()(const tiny::UUID& uuid) const
		{
			return (uint64_t)uuid;
		}
	};
}
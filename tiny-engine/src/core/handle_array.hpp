#pragma once

#include <vector>
#include <deque>

namespace tiny
{
	using handle_t = uint32_t;

	template<typename T>
	class HandleArray
	{
	public:
		handle_t add(const T& data)
		{
			if (!m_ExistsEmptySlots)
			{
				m_Data.push_back(data);
				return m_Data.size() - 1;
			}

			handle_t emptyHandle = m_EmptyData.back();
			m_EmptyData.pop_back();

			if (m_EmptyData.empty())
				m_ExistsEmptySlots = false;

			m_Data[emptyHandle] = data;
			return emptyHandle;
		}

		void remove(handle_t handle)
		{
			m_ExistsEmptySlots = true;
			m_EmptyData.push_back(handle);
			m_Data[handle] = {};
		}

	public:
		bool isValidHandle(handle_t handle) const
		{
			for (const auto& emptyHandle : m_EmptyData)
			{
				if (handle == emptyHandle)
					return false;
			}

			return true;
		}

	public:
		std::vector<T> data() const { return m_Data; }

	private:
		std::vector<T> m_Data;
		std::deque<handle_t> m_EmptyData;
		bool m_ExistsEmptySlots = false;
	};
}
#pragma once
#include "core/logger.hpp"

namespace tiny
{
	inline void __tiny_assert(const char* condition, const char* message, const char* file, uint64_t line)
	{
		TINY_LOG_CRITICAL("[Assert]: '{}' failed. [{}]\nFile: {} [{}]", condition, message, file, line);
	}

#define TINY_ASSERT(condition, message)									\
	{																		\
		if(condition) { }													\
		else																\
		{																	\
			tiny::__tiny_assert(#condition, message, __FILE__, __LINE__);		\
		}																	\
	}

#define TINY_ASS(condition)																\
	{																						\
		if(condition) { }																	\
		else																				\
		{																					\
			tiny::__tiny_assert(#condition, "No message provided", __FILE__, __LINE__);		\
		}																					\
	}
}
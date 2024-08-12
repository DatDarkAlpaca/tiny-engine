#pragma once
#include <numeric>
#include <limits>
#include <filesystem>

#include "core/defines.hpp"

#include "core/logger.hpp"
#include "core/assert.hpp"

#define BIND(function, argument) std::bind(function, argument, std::placeholders::_1)

#define EXPOSE_PROPERTY(ClassName, SetterName, PropertyName)						\
	const ClassName& SetterName() const { return PropertyName; }					\
	ClassName& SetterName() { return PropertyName; }

static inline constexpr uint32_t InvalidHandle = 0xFFFFFFFF;
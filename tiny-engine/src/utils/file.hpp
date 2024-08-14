#pragma once
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include "core/logger.hpp"

namespace tiny
{
	inline std::string readFile(const std::filesystem::path& filepath)
	{
		std::ifstream file(filepath);
		if (!file.is_open())
			TINY_LOG_ERROR("Failed to load file: {}", filepath.string());

		std::stringstream ss;

		ss << file.rdbuf();

		return ss.str();
	}

	inline std::vector<char> readBinaryFile(const std::filesystem::path& filepath)
	{
		std::ifstream file(filepath, std::ios::ate | std::ios::binary);

		if (!file.is_open())
			TINY_LOG_ERROR("Failed to load file: {}", filepath.string());

		size_t fileSize { static_cast<size_t>(file.tellg()) };

		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}
}
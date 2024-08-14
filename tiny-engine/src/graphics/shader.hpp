#pragma once
#include <glad/glad.h>
#include <filesystem>
#include <numeric>

#include "common.hpp"

namespace tiny
{
	using shader_handle = uint32_t;

	enum class ShaderType
	{
		UNKNOWN = 0,
		VERTEX,
		FRAGMENT
	};

    inline constexpr uint32_t getShaderTypeGL(ShaderType type)
    {
        switch (type)
        {
            case ShaderType::VERTEX:
                return GL_VERTEX_SHADER;

            case ShaderType::FRAGMENT:
                return GL_FRAGMENT_SHADER;    
        }

        return NULL;
    }

	struct ShaderDescriptor
	{
		std::filesystem::path filepath;
		ShaderType type;
	};

    inline void checkCompileErrors(uint32_t shader, ShaderType type)
    {
        int success;
        char infoLog[1024];

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            TINY_LOG_ERROR("Failed to compile shader: {}", infoLog);
        }
    }
}
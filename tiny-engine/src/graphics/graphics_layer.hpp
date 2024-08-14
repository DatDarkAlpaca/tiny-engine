#pragma once
#include <glad/glad.h>
#include <filesystem>
#include <vector>
#include <glm/glm.hpp>

#include "core/assets/asset_library.hpp"
#include "graphics.hpp"
#include "utils/file.hpp"

#include "shader.hpp"
#include "buffer.hpp"
#include "pipeline.hpp"
#include "texture.hpp"
#include "framebuffer.hpp"

namespace tiny
{
	using index_buffer_handle = uint32_t;
	using renderpass_handle = uint32_t;

	struct GraphicsLayer
	{
	public:
		GraphicsLayer()
		{
			glGenVertexArrays(1, &m_VAO);
		}

	public:
		void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			glViewport(x, y, width, height);
		}
		void setScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			glScissor(x, y, width, height);
		}

		void setClearColor(const glm::vec4& color)
		{
			glClearColor(color.r, color.b, color.g, color.a);
		}

		void begin()
		{
			glBindVertexArray(m_VAO);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		void draw(uint32_t first, uint32_t count)
		{
			auto descriptor = m_PipelineStates[m_BoundPipeline];
			glDrawArrays(getTopologyGL(descriptor.primitiveTopology), first, count);
		}

		void drawElements(uint32_t count)
		{
			auto descriptor = m_PipelineStates[m_BoundPipeline];
			glDrawElements(getTopologyGL(descriptor.primitiveTopology), count, GL_UNSIGNED_INT, nullptr);
		}

		void end()
		{
			glBindVertexArray(0);
		}

	public:
		shader_handle createShader(const ShaderDescriptor& descriptor)
		{
			std::string contents = readFile(descriptor.filepath);
			const char* cString = contents.c_str();

			uint32_t shaderID = glCreateShader(getShaderTypeGL(descriptor.type));
			glShaderSource(shaderID, 1, &cString, NULL);
			glCompileShader(shaderID);
			checkCompileErrors(shaderID, descriptor.type);

			m_Shaders.push_back(shaderID);
			return m_Shaders.size() - 1;
		}
		void destroyShader(shader_handle handle)
		{
			glDeleteShader(m_Shaders[handle]);
		}

		pipeline_handle createPipeline(const PipelineDescriptor& descriptor)
		{
			uint32_t pipelineProgramID = glCreateProgram();

			for (const auto& shaderHandle : descriptor.shaderHandles)
				glAttachShader(pipelineProgramID, m_Shaders[shaderHandle]);
			
			glLinkProgram(pipelineProgramID);

			checkLinkingErrors(pipelineProgramID);
			
			for (const auto& shaderHandle : descriptor.shaderHandles)
				destroyShader(shaderHandle);

			m_Pipelines.push_back(pipelineProgramID);
			m_PipelineStates.push_back(descriptor);

			return m_Pipelines.size() - 1;
		}
		void bindPipeline(pipeline_handle handle)
		{
			glUseProgram(m_Pipelines[handle]);
			setupPipelineState(handle);
			m_BoundPipeline = handle;
		}

		void setUniform(std::string_view uniformName, float value)
		{
			auto location = glGetUniformLocation(m_Pipelines[m_BoundPipeline], uniformName.data());
			glUniform1f(location, value);
		}

		void setUniform(std::string_view uniformName, int value)
		{
			auto location = glGetUniformLocation(m_Pipelines[m_BoundPipeline], uniformName.data());
			glUniform1i(location, value);
		}

		buffer_handle createBuffer(BufferDescriptor descriptor)
		{
			uint32_t buffer;
			glGenBuffers(1, &buffer);
			glBindBuffer(getBufferTypeGL(descriptor.type), buffer);
			glBufferData(getBufferTypeGL(descriptor.type), descriptor.size, nullptr, descriptor.usage);

			m_Buffers.push_back(buffer);
			return m_Buffers.size() - 1;
		}
		void bindBuffer(buffer_handle handle, BufferType type)
		{
			glBindBuffer(getBufferTypeGL(type), m_Buffers[handle]);
		}
		void setBufferData(buffer_handle handle, BufferDescriptor descriptor, void* data)
		{
			glBufferData(getBufferTypeGL(descriptor.type), descriptor.size, data, descriptor.usage);
		}

		texture_handle createTexture2DByte(AssetLibrary& assetLibrary, AssetHandle textureHandle, const TextureDescriptor& descriptor)
		{
			std::weak_ptr<TextureAsset> weakTextureData = assetLibrary.getTexture(textureHandle);
			auto textureData = weakTextureData.lock();

			uint32_t textureID;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(
				GL_TEXTURE_2D, 
				0, 
				GL_RGBA,
				textureData->width,
				textureData->height,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				textureData->buffer.data
			);

			// TODO: parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

			glGenerateMipmap(GL_TEXTURE_2D);

			m_Texture.push_back(textureID);
			return m_Texture.size() - 1;
		}
		void bindTexture2D(texture_handle handle, uint32_t unit = 0)
		{
			glActiveTexture(GL_TEXTURE0 + unit);
			glBindTexture(GL_TEXTURE_2D, m_Texture[handle]);
		}

		framebuffer_handle createFramebuffer()
		{
			uint32_t framebuffer;
			glGenFramebuffers(1, &framebuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

			/*if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				TINY_LOG_CRITICAL("FUCK FRAMEBUFFERS");*/
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		void attachFramebufferColorTexture(framebuffer_handle handle, texture_handle textureHandle, uint32_t attachmentID = 0)
		{
			bindFramebuffer(handle);
			glFramebufferTexture2D(
				GL_FRAMEBUFFER, 
				GL_COLOR_ATTACHMENT0 + attachmentID, 
				GL_TEXTURE_2D, 
				m_Texture[textureHandle], 
				0
			);
		}
		void bindFramebuffer(framebuffer_handle handle)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffers[handle]);
		}

	private:
		void setupPipelineState(pipeline_handle handle)
		{
			auto descriptor = m_PipelineStates[handle];

			// Vertex Layout:
			size_t offset = 0, stride = 0;

			for (const auto& element : descriptor.vertexLayout.elements)
				stride += element.dataAmount * getDataTypeSize(element.dataType);

			for(const auto& element : descriptor.vertexLayout.elements)
			{
				glEnableVertexAttribArray(element.location);
				glVertexAttribPointer(
					element.location,
					element.dataAmount,
					getDataTypeGL(element.dataType),
					false,
					stride,
					(void*)offset);

				offset += element.dataAmount * getDataTypeSize(element.dataType);
			}

			glFrontFace(getFrontFaceGL(descriptor.frontFace));
			glPolygonMode(GL_FRONT_AND_BACK, getPolygonModeGL(descriptor.polygonMode));
			glCullFace(getCullModeGL(descriptor.cullMode));
		}

	private:
		std::vector<shader_handle> m_Shaders;
		std::vector<pipeline_handle> m_Pipelines;
		std::vector<buffer_handle> m_Buffers;
		std::vector<PipelineDescriptor> m_PipelineStates;
		std::vector<texture_handle> m_Texture;
		std::vector<framebuffer_handle> m_Framebuffers;

	private:
		pipeline_handle m_BoundPipeline;
		uint32_t m_VAO = 0;
	};
}
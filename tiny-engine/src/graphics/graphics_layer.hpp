#pragma once
#include <glad/glad.h>
#include <filesystem>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/assets/asset_library.hpp"
#include "graphics.hpp"
#include "utils/file.hpp"

#include "handle.hpp"
#include "shader.hpp"
#include "buffer.hpp"
#include "pipeline.hpp"
#include "texture.hpp"
#include "framebuffer.hpp"

namespace tiny
{
	// TODO: uniform name caching
	// TODO: support for other buffer types in bindings?

	struct GraphicsLayer
	{
	public:
		GraphicsLayer()
		{
			unsigned int vao;
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);
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

		void clear()
		{
			glClear(GL_COLOR_BUFFER_BIT);
		}

		// TODO: recording should be done on a command buffer.
		// - maybe use vaos per pipeline and set it up beforehand
		void begin()
		{
			m_RecordingEnabled = true;
			m_BufferBindings.clear();
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
		void drawInstanced(uint32_t first, uint32_t count, uint32_t instanceCount)
		{
			auto descriptor = m_PipelineStates[m_BoundPipeline];
			glDrawArraysInstanced(getTopologyGL(descriptor.primitiveTopology), first, count, instanceCount);
		}
		void drawElementsInstanced(uint32_t count, uint32_t instanceCount)
		{
			auto descriptor = m_PipelineStates[m_BoundPipeline];
			glDrawElementsInstanced(
				getTopologyGL(descriptor.primitiveTopology), 
				count, 
				GL_UNSIGNED_INT, 
				nullptr, 
				instanceCount
			);
		}

		void end()
		{
			m_RecordingEnabled = false;
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
			return shader_handle(m_Shaders.size() - 1);
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

			return pipeline_handle(m_Pipelines.size() - 1);
		}
		void bindPipeline(pipeline_handle handle)
		{
			if (m_BoundPipeline == handle)
				return;

			m_BoundPipeline = handle;

			glUseProgram(m_Pipelines[handle]);

			setupPipelineState(handle);
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
		void setUniform(std::string_view uniformName, const glm::vec2& value)
		{
			auto location = glGetUniformLocation(m_Pipelines[m_BoundPipeline], uniformName.data());
			glUniform2f(location, value.x, value.y);
		}
		void setUniform(std::string_view uniformName, const glm::vec3& value)
		{
			auto location = glGetUniformLocation(m_Pipelines[m_BoundPipeline], uniformName.data());
			glUniform3f(location, value.x, value.y, value.z);
		}
		void setUniform(std::string_view uniformName, const glm::vec4& value)
		{
			auto location = glGetUniformLocation(m_Pipelines[m_BoundPipeline], uniformName.data());
			glUniform4f(location, value.x, value.y, value.z, value.w);
		}
		void setUniform(std::string_view uniformName, const glm::mat4& value)
		{
			auto location = glGetUniformLocation(m_Pipelines[m_BoundPipeline], uniformName.data());
			glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
		}

		buffer_handle createBuffer(BufferDescriptor descriptor)
		{
			uint32_t buffer;
			glGenBuffers(1, &buffer);
			glBindBuffer(getBufferTypeGL(descriptor.type), buffer);
			glBufferData(getBufferTypeGL(descriptor.type), descriptor.size, nullptr, descriptor.usage);

			glBindBuffer(getBufferTypeGL(descriptor.type), 0);

			m_Buffers.push_back(buffer);
			return buffer_handle(m_Buffers.size() - 1);
		}
		void unbindBuffer(BufferType type)
		{
			glBindBuffer(getBufferTypeGL(type), 0);
		}
		void bindBuffer(buffer_handle handle, BufferType type)
		{
			if (m_RecordingEnabled && type == BufferType::ARRAY_BUFFER)
				m_BufferBindings.push_back(m_Buffers[handle]);

			glBindBuffer(getBufferTypeGL(type), m_Buffers[handle]);
		}
		void setBufferData(buffer_handle handle, BufferDescriptor descriptor, void* data)
		{
			bindBuffer(handle, descriptor.type);
			
			glBufferData(getBufferTypeGL(descriptor.type), descriptor.size, data, descriptor.usage);

			unbindBuffer(descriptor.type);
		}

		texture_handle createTexture2DByteNull(uint32_t width, uint32_t height)
		{
			uint32_t textureID;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				nullptr
			);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glBindTexture(GL_TEXTURE_2D, 0);

			m_Texture.push_back(textureID);
			return texture_handle(m_Texture.size() - 1);
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

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getTextureFilterGL(descriptor.minFilter));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, getTextureFilterGL(descriptor.magFilter));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, getTextureClampGL(descriptor.wrapS));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, getTextureClampGL(descriptor.wrapT));

			if (descriptor.generateBipmap)
				glGenerateMipmap(GL_TEXTURE_2D);
			
			glBindTexture(GL_TEXTURE_2D, 0);

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

			m_Framebuffers.push_back(framebuffer);
			return framebuffer_handle(m_Framebuffers.size() - 1);
		}
		uint32_t getFramebufferID(framebuffer_handle handle) const
		{
			return m_Framebuffers[handle];
		}
		void attachFramebufferDepthStencil(framebuffer_handle handle)
		{
			bindFramebuffer(handle);

			unsigned int rbo;
			glGenRenderbuffers(1, &rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
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
		void bindDefaultFramebuffer()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

	private:
		void setupPipelineState(pipeline_handle handle)
		{
			auto descriptor = m_PipelineStates[handle];

			// Vertex Layout:
			size_t biggestBinding = 0;
			{
				// Fetching the biggest binding:
				for (const auto& element : descriptor.vertexLayout.elements)
				{
					if (element.binding > biggestBinding)
						biggestBinding = element.binding;
				}
			}
			
			// assuming the user is not stupid, he'll write bindings starting at zero and counting up
			// TODO: account for user stupidity.
			std::vector<size_t> offsets(biggestBinding + 1, 0), strides(biggestBinding + 1, 0);
			{
				for (const auto& element : descriptor.vertexLayout.elements)
					strides[element.binding] += element.dataAmount * getDataTypeSize(element.dataType);
			}

			uint32_t boundBuffer = invalid_handle;
			for(const auto& element : descriptor.vertexLayout.elements)
			{
				glEnableVertexAttribArray(element.location);

				glBindBuffer(getBufferTypeGL(BufferType::ARRAY_BUFFER), m_BufferBindings[element.binding]);

				glVertexAttribPointer(
					element.location,
					element.dataAmount,
					getDataTypeGL(element.dataType),
					false,
					strides[static_cast<size_t>(element.binding)],
					(void*)offsets[element.binding]
				);

				uint32_t divisor = element.inputRate == InputRate::PER_INSTANCE ? 1 : 0;
				glVertexAttribDivisor(element.location, divisor);

				offsets[element.binding] += element.dataAmount * getDataTypeSize(element.dataType);
			}

			// Rasterizer:
			glFrontFace(getFrontFaceGL(descriptor.frontFace));
			glPolygonMode(GL_FRONT_AND_BACK, getPolygonModeGL(descriptor.polygonMode));
			glCullFace(getCullModeGL(descriptor.cullMode));
		}

	private:
		std::vector<uint32_t> m_Shaders;
		
		std::vector<uint32_t> m_Buffers;

		std::vector<uint32_t> m_Pipelines;
		std::vector<PipelineDescriptor> m_PipelineStates;

		std::vector<uint32_t> m_Texture;
		std::vector<uint32_t> m_Framebuffers;
		
		std::vector<uint32_t> m_Renderpass;
		
	private:
		bool m_RecordingEnabled = false;
		std::vector<uint32_t> m_BufferBindings;

	private:
		pipeline_handle m_BoundPipeline = invalid_handle;
	};
}
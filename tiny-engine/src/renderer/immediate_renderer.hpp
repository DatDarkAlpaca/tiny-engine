#pragma once
#include "scene/scene.hpp"

#include "graphics/camera.hpp"
#include "graphics/graphics_layer.hpp"

namespace tiny
{
	class ImmediateSpriteRenderer
	{
	public:
		void initialize(GraphicsLayer* layer)
		{
			r_Graphics = layer;

			initializeBuffers();

			initializePipeline();
		}

		void render(Scene& scene, ICamera* camera)
		{
			r_Graphics->begin();

			if (m_RenderTarget == invalid_handle)
				r_Graphics->bindDefaultFramebuffer();
			else
				r_Graphics->bindFramebuffer(m_RenderTarget);

			r_Graphics->clear();

			r_Graphics->bindBuffer(m_VBO, BufferType::ARRAY_BUFFER);
			r_Graphics->bindBuffer(m_EBO, BufferType::ELEMENT_ARRAY_BUFFER);

			r_Graphics->bindPipeline(m_DefaultPipeline);

			r_Graphics->setUniform("u_texture", 0);
			r_Graphics->setUniform("u_view", camera->getView());
			r_Graphics->setUniform("u_projection", camera->getProjection());

			auto view = scene.registry().view<TransformComponent, SpriteComponent>();
			for (auto [_, transformComponent, spriteComponent] : view.each()) 
			{
				glm::mat4 transform = transformComponent.transform;
				texture_handle texture = spriteComponent.textureHandle;

				renderSprite(transform, texture);
			}

			r_Graphics->end();
		}

	public:
		void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			r_Graphics->setViewport(x, y, width, height);
		}

		void setRenderTarget(framebuffer_handle handle)
		{
			m_RenderTarget = handle;
		}

		void resetRenderTarget()
		{
			m_RenderTarget = invalid_handle;
		}

	private:
		void renderSprite(const glm::mat4& transform, texture_handle textureHandle)
		{
			r_Graphics->setUniform("u_model", transform);

			r_Graphics->bindTexture2D(textureHandle);

			r_Graphics->drawElements(6);
		}

	private:
		void initializeBuffers()
		{
			// Quad VBO:
			{
				std::vector<float> data = {
					-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,
					 1.0f, -1.0f, 0.0f,		1.0f, 0.0f,
					 1.0f,  1.0f, 0.0f,		1.0f, 1.0f,
					-1.0f,  1.0f, 0.0f,		0.0f, 1.0f,
				};

				BufferDescriptor descriptor;
				descriptor.type = BufferType::ARRAY_BUFFER;
				descriptor.usage = GL_STATIC_DRAW;
				descriptor.size = sizeof(float) * data.size();

				m_VBO = r_Graphics->createBuffer(descriptor);
				r_Graphics->setBufferData(m_VBO, descriptor, data.data());
			}

			// Quad EBO:
			{
				std::vector<unsigned int> data = {
				0, 1, 2, 2, 3, 0
				};

				BufferDescriptor descriptor;
				descriptor.type = BufferType::ELEMENT_ARRAY_BUFFER;
				descriptor.usage = GL_STATIC_DRAW;
				descriptor.size = sizeof(unsigned int) * data.size();

				m_EBO = r_Graphics->createBuffer(descriptor);
				r_Graphics->setBufferData(m_EBO, descriptor, data.data());
			}
		}

		void initializePipeline()
		{
			// Shaders:
			auto vertex = r_Graphics->createShader({ "res/sprite_shader.vert", ShaderType::VERTEX });
			auto fragment = r_Graphics->createShader({ "res/sprite_shader.frag", ShaderType::FRAGMENT });

			// Default Pipeline:
			{
				PipelineDescriptor pipelineDesc;

				pipelineDesc.vertexLayout.add(VertexLayout::Element{ 0, 0, 3, DataType::FLOAT });
				pipelineDesc.vertexLayout.add(VertexLayout::Element{ 0, 1, 2, DataType::FLOAT });
				pipelineDesc.shaderHandles.push_back(vertex);
				pipelineDesc.shaderHandles.push_back(fragment);

				m_DefaultPipeline = r_Graphics->createPipeline(pipelineDesc);
			}
		}

	private:
		GraphicsLayer* r_Graphics = nullptr;

	private:
		framebuffer_handle m_RenderTarget = invalid_handle;

		buffer_handle
			m_VBO = invalid_handle,
			m_EBO = invalid_handle;

		pipeline_handle m_DefaultPipeline = invalid_handle;
	};
}
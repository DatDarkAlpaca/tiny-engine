#include "pch.hpp"
#include <tiny_engine.hpp>

using namespace tiny;

class MainGame : public View
{
public:
	explicit MainGame() = default;

public:
	void onEngineStartup() override
	{
		// Asset:
		TextureAsset asset = TextureLoader::load("res/box.png");
		AssetHandle handle = assetLibrary().insertTexture(asset);

		// Graphics:
		auto vertex = graphics.createShader({ "res/shader.vert", ShaderType::VERTEX });
		auto fragment = graphics.createShader({ "res/shader.frag", ShaderType::FRAGMENT });

		auto invertVertex = graphics.createShader({ "res/invert.vert", ShaderType::VERTEX });
		auto invertFragment = graphics.createShader({ "res/invert.frag", ShaderType::FRAGMENT });

		auto grayscaleVertex = graphics.createShader({ "res/grayscale.vert", ShaderType::VERTEX });
		auto grayscaleFragment = graphics.createShader({ "res/grayscale.frag", ShaderType::FRAGMENT });

		// VBO:
		{
			std::vector<float> data = {
				-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
				 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
				 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
				-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
			};

			BufferDescriptor descriptor;
			descriptor.type  = BufferType::ARRAY_BUFFER;
			descriptor.usage = GL_STATIC_DRAW;
			descriptor.size = sizeof(float) * data.size();

			vbo = graphics.createBuffer(descriptor);
			graphics.setBufferData(vbo, descriptor, data.data());
		}

		// Offset VBO:
		{
			std::vector<float> data = {
				0.f,  0.f,
				1.5f, 0.f
			};

			BufferDescriptor descriptor;
			descriptor.type = BufferType::ARRAY_BUFFER;
			descriptor.usage = GL_STATIC_DRAW;
			descriptor.size = sizeof(float) * data.size();

			offsetVBO = graphics.createBuffer(descriptor);
			graphics.setBufferData(offsetVBO, descriptor, data.data());
		}

		// EBO:
		{
			std::vector<unsigned int> data = {
				0, 1, 2, 2, 3, 0
			};

			BufferDescriptor descriptor;
			descriptor.type = BufferType::ELEMENT_ARRAY_BUFFER;
			descriptor.usage = GL_STATIC_DRAW;
			descriptor.size = sizeof(unsigned int) * data.size();

			ebo = graphics.createBuffer(descriptor);
			graphics.setBufferData(ebo, descriptor, data.data());
		}

		// Screen VBO:
		{
			std::vector<float> data = {
				-1.0f, -1.0f, 0.0f, 0.0f,
				 1.0f, -1.0f, 1.0f, 0.0f,
				 1.0f,  1.0f, 1.0f, 1.0f,

				 1.0f,  1.0f, 1.0f, 1.0f,
				-1.0f,  1.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f,
			};

			BufferDescriptor descriptor;
			descriptor.type = BufferType::ARRAY_BUFFER;
			descriptor.usage = GL_STATIC_DRAW;
			descriptor.size = sizeof(float) * data.size();

			screenVBO = graphics.createBuffer(descriptor);
			graphics.setBufferData(screenVBO, descriptor, data.data());
		}

		// Default Pipeline:
		{
			PipelineDescriptor pipelineDesc;

			pipelineDesc.vertexLayout.add(VertexLayout::Element{ 0, 0, 3, DataType::FLOAT });
			pipelineDesc.vertexLayout.add(VertexLayout::Element{ 0, 1, 2, DataType::FLOAT });
			pipelineDesc.vertexLayout.add(VertexLayout::Element{ 1, 2, 2, DataType::FLOAT, InputRate::PER_INSTANCE });
			pipelineDesc.shaderHandles.push_back(vertex);
			pipelineDesc.shaderHandles.push_back(fragment);

			colorPipeline = graphics.createPipeline(pipelineDesc);
		}

		// Invert Pipeline:
		{
			PipelineDescriptor pipelineDesc;

			pipelineDesc.vertexLayout.add(VertexLayout::Element{ 0, 0, 2, DataType::FLOAT });
			pipelineDesc.vertexLayout.add(VertexLayout::Element{ 0, 1, 2, DataType::FLOAT });

			pipelineDesc.shaderHandles.push_back(invertVertex);
			pipelineDesc.shaderHandles.push_back(invertFragment);

			invertPipeline = graphics.createPipeline(pipelineDesc);
		}

		// Grayscale Pipeline:
		{
			PipelineDescriptor pipelineDesc;

			pipelineDesc.vertexLayout.add(VertexLayout::Element{ 0, 0, 2, DataType::FLOAT });
			pipelineDesc.vertexLayout.add(VertexLayout::Element{ 0, 1, 2, DataType::FLOAT });
			pipelineDesc.shaderHandles.push_back(grayscaleVertex);
			pipelineDesc.shaderHandles.push_back(grayscaleFragment);

			grayscalePipeline = graphics.createPipeline(pipelineDesc);
		}
		
		texture = graphics.createTexture2DByte(application->getAssetLibrary(), handle, {});

		fboTexture = graphics.createTexture2DByteNull(640, 480);
		fbo = graphics.createFramebuffer();
		graphics.attachFramebufferColorTexture(fbo, fboTexture, 0);
		graphics.attachFramebufferDepthStencil(fbo);
		
		graphics.setClearColor({ 0.5f, 0.5f, 0.5f, 1.0f });
	}

	void onRender() override
	{
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(150.f, 150.f, 0.f));
		model = glm::scale(model, glm::vec3(200.f, 200.f, 1.f));

		glm::mat4 view(1.0f);
		view = glm::lookAt(
			glm::vec3(0.f, 0.f, 0.f), 
			glm::vec3(0.f, 0.f, -1.f), 
			glm::vec3(0.f, 1.f, 0.f)
		);

		glm::mat4 projection(1.0f);
		projection = glm::ortho(0.f, 640.f, 480.f, 0.f, -1.f, 1.f);

		// Color pass:
		graphics.begin();
		{
			graphics.setViewport(0, 0, 640, 480);
			graphics.bindDefaultFramebuffer();
			graphics.clear();

			// the order of binding matters.
			graphics.bindBuffer(vbo, BufferType::ARRAY_BUFFER);
			graphics.bindBuffer(offsetVBO, BufferType::ARRAY_BUFFER);
			graphics.bindBuffer(ebo, BufferType::ELEMENT_ARRAY_BUFFER);

			{
				graphics.bindPipeline(colorPipeline);

				// Array Instancing:
				graphics.setUniform("u_texture", 0);
				graphics.setUniform("u_model", model);
				graphics.setUniform("u_view", view);
				graphics.setUniform("u_projection", projection);

				graphics.bindTexture2D(texture);

				graphics.drawElementsInstanced(6, 2);
			}
		}
		graphics.end();

		return;

		// Invert pass:
		{
			graphics.setViewport(0, 0, 640, 480);
			graphics.bindDefaultFramebuffer();
			graphics.clear();

			graphics.bindBuffer(screenVBO, BufferType::ARRAY_BUFFER);

			{
				graphics.bindPipeline(invertPipeline);

				graphics.setUniform("u_texture_screen", 0);

				graphics.bindTexture2D(fboTexture);

				graphics.draw(0, 6);
			}
		}

		// Grayscale pass:
		{
			graphics.setViewport(0, 0, 640, 480);
			graphics.bindDefaultFramebuffer();
			graphics.clear();

			graphics.bindBuffer(screenVBO, BufferType::ARRAY_BUFFER);

			{
				graphics.bindPipeline(grayscalePipeline);

				graphics.setUniform("u_texture_screen", 0);

				graphics.bindTexture2D(fboTexture);

				graphics.draw(0, 6);
			}		
		}
	}

private:
	GraphicsLayer graphics;

private:
	framebuffer_handle fbo = invalid_handle;
	buffer_handle 
		vbo			= invalid_handle,
		offsetVBO	= invalid_handle,
		ebo			= invalid_handle, 
		screenVBO	= invalid_handle;
	
	pipeline_handle colorPipeline = invalid_handle;
	pipeline_handle invertPipeline = invalid_handle;
	pipeline_handle grayscalePipeline = invalid_handle;

	texture_handle texture = invalid_handle, fboTexture = invalid_handle;
};

class Testbed : public TinyApplication
{
public:
	Testbed(const WindowProperties& properties)
		: TinyApplication(properties)
	{
		addView(std::make_unique<MainGame>());
	}
};

std::unique_ptr<tiny::TinyApplication> createApplication()
{
	return std::make_unique<Testbed>(WindowProperties{ "Testbed" });
}
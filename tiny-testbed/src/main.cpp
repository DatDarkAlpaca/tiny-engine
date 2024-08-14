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

		// - buffer:
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

		{
			PipelineDescriptor pipelineDesc;

			pipelineDesc.vertexLayout.add(VertexLayout::Element{ 0, 3, DataType::FLOAT });
			pipelineDesc.vertexLayout.add(VertexLayout::Element{ 1, 2, DataType::FLOAT });
			pipelineDesc.shaderHandles.push_back(vertex);
			pipelineDesc.shaderHandles.push_back(fragment);

			pipeline = graphics.createPipeline(pipelineDesc);
		}
		
		texture = graphics.createTexture2DByte(application->getAssetLibrary(), handle, {});

		graphics.setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	}

	void onRender() override
	{
		graphics.begin();
		graphics.bindPipeline(pipeline);
		graphics.bindBuffer(vbo, BufferType::ARRAY_BUFFER);
		graphics.bindBuffer(ebo, BufferType::ELEMENT_ARRAY_BUFFER);

		graphics.setUniform("u_texture", 0);
		graphics.bindTexture2D(texture);

		graphics.drawElements(6);

		graphics.end();
	}

private:
	Scene m_Scene;

private:
	GraphicsLayer graphics;

	buffer_handle vbo, ebo;
	pipeline_handle pipeline;
	texture_handle texture;
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
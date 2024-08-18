#include "pch.hpp"
#include <tiny_engine.hpp>

using namespace tiny;

// TODO: scene framebuffer resizing

class MainGame : public View
{
public:
	explicit MainGame() = default;

public:
	void onEngineStartup() override
	{
		initializeScene();

		initializeCamera();

		spriteRenderer.initialize(&graphics);
	}

	void onRender() override
	{
		m_Camera.updateView();
		
		spriteRenderer.setRenderTarget(m_SceneFramebuffer);

		spriteRenderer.render(m_Scene, &m_Camera);
	}

	void onRenderGUI() override
	{
		// Main Scene:
		graphics.bindDefaultFramebuffer();
		{
			ImGui::DockSpaceOverViewport(ImGuiDockNodeFlags_PassthruCentralNode, ImGui::GetMainViewport());
			ImGui::Begin("Main Scene");

			glm::vec2 pos = { 0.f, 0.f };
			ImGui::Image(
				(void*)(graphics.getTextureID(m_SceneFramebufferTexture)), 
				ImGui::GetContentRegionAvail(),
				ImVec2(0, 1), 
				ImVec2(1, 0)
			);

			auto size = ImGui::GetWindowSize();
			m_Camera.updateProjection(0.f, float(size.x), float(size.y), 0.f);

			ImGui::End();

			graphics.clear();
		}
	}

private:
	void initializeScene()
	{
		int width, height;
		glfwGetWindowSize(application->getWindowModule().getHandle(), &width, &height);

		// Box Grid:
		TextureAsset asset = TextureLoader::load("res/box.png");
		AssetHandle boxGridHandle = assetLibrary().insertTexture(asset);

		texture_handle texture = graphics.createTexture2DByte(application->getAssetLibrary(), boxGridHandle, {});
		
		size_t scale = 60;
		for (size_t x = 0; x < 2; ++x)
		{
			glm::mat4 transform(1.0f);
			transform = glm::translate(transform, glm::vec3(scale + x * 2 * scale, scale, 0.f));
			transform = glm::scale(transform, glm::vec3(scale, scale, 1.f));

			auto box = m_Scene.addEntity();
			box.AddComponent<TransformComponent>(transform);
			box.AddComponent<SpriteComponent>(texture);
		}

		createSceneFramebuffer(width, height);
		m_SceneDimensions = { width, height };

		m_Camera.updateProjection(0.0f, m_SceneDimensions.x, m_SceneDimensions.y, 0.f, -10.f, 10.f);
	}

	void initializeCamera()
	{
		m_Camera.position = glm::vec3(0.f);
		m_Camera.target   = glm::vec3(0.f, 0.f, -1.f);
	}

	void createSceneFramebuffer(uint32_t width, uint32_t height)
	{
		m_SceneDimensions = { width, height };

		if (m_SceneFramebufferTexture != invalid_handle)
			graphics.destroyTexture(m_SceneFramebufferTexture);

		if (m_SceneFramebuffer != invalid_handle)
			graphics.destroyFramebuffer(m_SceneFramebuffer);

		m_SceneFramebufferTexture = graphics.createTexture2DByteNull(width, height);

		m_SceneFramebuffer = graphics.createFramebuffer();
		graphics.attachFramebufferColorTexture(m_SceneFramebuffer, m_SceneFramebufferTexture);
		graphics.attachFramebufferDepthStencil(m_SceneFramebuffer);
	}

private:
	ImmediateSpriteRenderer spriteRenderer;
	OrthographicCamera m_Camera;
	GraphicsLayer graphics;
	Scene m_Scene;

private:
	framebuffer_handle m_SceneFramebuffer = invalid_handle;
	texture_handle m_SceneFramebufferTexture = invalid_handle;
	glm::vec2 m_SceneDimensions = glm::vec2(0.f);
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
#pragma once
#include "core/view.hpp"
#include "core/logger.hpp"
#include "core/worker_pool.hpp"
#include "core/window_module.hpp"
#include "core/assets/asset_library.hpp"

namespace tiny
{
	class TinyApplication
	{
	public:
		explicit TinyApplication(const WindowProperties& properties)
		{
			LoggerSystem::initialize();

			setupWindowing(m_WindowModule, properties);
			m_WindowModule.setEventCallback(BIND(&TinyApplication::onEvent, this));
		}

		~TinyApplication()
		{
			m_WindowModule.shutdown();
		}

	public:
		void run()
		{
			onEngineStartup();

			m_PreviousTime = glfwGetTime();
			while (m_WindowModule.isOpen())
			{
				// Calculate delta time
				double currentTime = glfwGetTime();
				double deltaTime = currentTime - m_PreviousTime;
				m_PreviousTime = currentTime;

				m_WindowModule.pollEvents();

				onUpdate(deltaTime);

				onRender();

				onRenderGUI();

				m_WindowModule.swapBuffers();
			}

			onEngineShutdown();
		}

		void addView(std::unique_ptr<View> view)
		{
			view->setEngine(this);
			m_ViewSystem.add(std::move(view));
		}

	private:
		void onEngineStartup()
		{
			for (const auto& view : m_ViewSystem)
				view->onEngineStartup();
		}

		void onRender()
		{
			for (const auto& view : m_ViewSystem)
				view->onRender();
		}

		void onRenderGUI()
		{
			for (const auto& view : m_ViewSystem)
				view->onRenderGUI();
		}

		void onUpdate(double deltaTime)
		{
			for (const auto& view : m_ViewSystem)
				view->onUpdate(deltaTime);
		}

		void onEvent(Event& event)
		{
			for (const auto& view : m_ViewSystem)
				view->onEvent(event);
		}

		void onEngineShutdown()
		{
			for (const auto& view : m_ViewSystem)
				view->onEngineShutdown();
		}

	public:
		inline WindowModule& getWindowModule() { return m_WindowModule; }

		inline WorkerPool& getWorkerPool() { return m_WorkerPool; }

		inline AssetLibrary& getAssetLibrary() { return m_AssetLibrary; }

	private:
		WindowModule m_WindowModule;
		WorkerPool m_WorkerPool;
		AssetLibrary m_AssetLibrary;
		ViewSystem m_ViewSystem;

	private:
		double m_PreviousTime = 0.0;
	};
}
#pragma once
#include "common.hpp"
#include "core/event/events.hpp"

namespace tiny
{
	class TinyApplication;

	class WorkerPool;
	class ViewSystem;
	class AssetLibrary;

	class View
	{
	public:
		virtual ~View() = default;

	public:
		virtual void onEngineStartup() { };

		virtual void onViewInstantiated() { };

		virtual void onRender() { };

		virtual void onRenderGUI() { };

		virtual void onUpdate(double deltaTime) { };

		virtual void onEvent(Event& event) { };

		virtual void onViewRemoved() { };

		virtual void onEngineShutdown() { };

	public:
		void setEngine(TinyApplication* application) { this->application = application; }

		void setViewSystem(ViewSystem* viewSystem) { this->viewSystem = viewSystem; }

	public:
		AssetLibrary& assetLibrary() const;

		WorkerPool& workerPool() const;

	protected:
		TinyApplication* application = nullptr;
		ViewSystem* viewSystem = nullptr;
	};

	using ViewHandle = size_t;

	class ViewSystem
	{
		using ViewContainer = std::vector<std::unique_ptr<View>>;

	public:
		ViewHandle add(std::unique_ptr<View> view)
		{
			view->onViewInstantiated();
			view->setViewSystem(this);

			m_Views.push_back(std::move(view));

			return m_Views.size() - 1;
		}

		void remove(ViewHandle handle)
		{
			auto it = m_Views.begin() + handle;
			it->get()->onViewRemoved();

			m_Views.erase(it);
		}

		View& get(ViewHandle handle)
		{
			TINY_ASSERT(m_Views.size() - 1 <= handle, "Invalid view index selected");
			return *m_Views[handle].get();
		}

	public:
		ViewContainer::reverse_iterator rbegin() { return m_Views.rbegin(); }
		ViewContainer::reverse_iterator rend() { return m_Views.rend(); }
		ViewContainer::iterator begin() { return m_Views.begin(); }
		ViewContainer::iterator end() { return m_Views.end(); }

		ViewContainer::const_reverse_iterator rbegin() const { return m_Views.rbegin(); }
		ViewContainer::const_reverse_iterator rend() const { return m_Views.rend(); }
		ViewContainer::const_iterator begin() const { return m_Views.begin(); }
		ViewContainer::const_iterator end()	const { return m_Views.end(); }

	private:
		std::vector<std::unique_ptr<View>> m_Views;
	};
}
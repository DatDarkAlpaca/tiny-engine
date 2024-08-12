#include "pch.hpp"
#include "view.hpp"
#include "core/application.hpp"

namespace tiny
{
	AssetLibrary& View::AssetLibrary() const
	{
		return application->getAssetLibrary();
	}

	WorkerPool& View::WorkerPool() const
	{
		return application->getWorkerPool();
	}
}


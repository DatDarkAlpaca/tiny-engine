#include "pch.hpp"
#include "view.hpp"
#include "core/application.hpp"

namespace tiny
{
	AssetLibrary& View::assetLibrary() const
	{
		return application->getAssetLibrary();
	}

	WorkerPool& View::workerPool() const
	{
		return application->getWorkerPool();
	}
}


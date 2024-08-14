#pragma once
#include <condition_variable>
#include <functional>
#include <future>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <tuple>
#include <queue>

#include "common.hpp"

namespace tiny
{
	class WorkerPool
	{
	public:
		explicit WorkerPool(size_t threadAmount = std::thread::hardware_concurrency())
		{
			for (size_t i = 0; i < threadAmount; ++i)
				m_Workers.emplace_back(&WorkerPool::workerThread, this);
		}

		~WorkerPool()
		{
			{
				std::unique_lock<std::mutex> lock(m_TaskQueueMutex);
				m_StopExecution = true;
			}

			m_TaskCondition.notify_all();

			for (std::thread& worker : m_Workers)
				worker.join();
		}

	public:
		template<typename Function, typename... Args, typename Callback>
		void submitTask(Function&& function, std::tuple<Args&...>&& args, Callback&& callback)
		{
			using returnType = typename std::invoke_result<Function, Args...>::type;

			auto task = std::make_shared<std::packaged_task<returnType()>>(
				[function, args = std::move(args)]() mutable {
					return std::apply(function, args);
				}
			);

			{
				std::unique_lock<std::mutex> lock(m_TaskQueueMutex);
				if (m_StopExecution)
					TINY_LOG_CRITICAL("[WorkerPool]: The worker pool has stopped suddenly.");

				m_Tasks.emplace([task, callback, this]()
					{
						(*task)();
						returnType taskResult = task->get_future().get();

						{
							std::unique_lock<std::mutex> lock(m_CompletedTasksMutex);
							m_CompletedTasks.emplace([callback, taskResult]() { callback(taskResult); });
						}
					});
			}

			m_TaskCondition.notify_one();
		}

		void pollTasks()
		{
			std::queue<std::function<void()>> tasksToExecute;

			{
				std::unique_lock<std::mutex> lock(m_CompletedTasksMutex);
				std::swap(tasksToExecute, m_CompletedTasks);
			}

			while (!tasksToExecute.empty())
			{
				tasksToExecute.front()();
				tasksToExecute.pop();
			}
		}

	private:
		void workerThread()
		{
			while (true)
			{
				std::function<void()> task;

				{
					std::unique_lock<std::mutex> lock(m_TaskQueueMutex);
					m_TaskCondition.wait(lock, [this] { return m_StopExecution || !m_Tasks.empty(); });

					if (m_StopExecution && m_Tasks.empty())
						return;

					task = std::move(m_Tasks.front());
					m_Tasks.pop();
				}

				task();
			}
		}

	private:
		std::vector<std::thread> m_Workers;
		std::queue<std::function<void()>> m_Tasks;
		bool m_StopExecution = false;

		std::mutex m_TaskQueueMutex;
		std::condition_variable m_TaskCondition;

		std::queue<std::function<void()>> m_CompletedTasks;
		std::mutex m_CompletedTasksMutex;
	};
}
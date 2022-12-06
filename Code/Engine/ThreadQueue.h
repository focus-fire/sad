#pragma once

#include <thread>
#include <vector>
#include <functional>

namespace sad
{
	class ThreadQueue
	{
	public:
		ThreadQueue() = default;
		~ThreadQueue() = default;

	public:
		void SubmitToApplicationThreadQueue(const std::function<void()>& function);

	public:
		void ExecuteApplicationThreadQueue();

	private:
		std::vector<std::function<void()>> m_ApplicationThreadQueue;
		std::mutex m_ApplicationThreadQueueMutex;
	};
}

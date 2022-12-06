#include "sadpch.h"

#include "ThreadQueue.h"

void sad::ThreadQueue::SubmitToApplicationThreadQueue(const std::function<void()>& function)
{
	std::scoped_lock<std::mutex> lock = std::scoped_lock<std::mutex>(m_ApplicationThreadQueueMutex);

	m_ApplicationThreadQueue.emplace_back(function);
}

void sad::ThreadQueue::ExecuteApplicationThreadQueue()
{
	std::scoped_lock<std::mutex> lock = std::scoped_lock<std::mutex>(m_ApplicationThreadQueueMutex);

	for (auto& function : m_ApplicationThreadQueue)
		function();

	m_ApplicationThreadQueue.clear();
}

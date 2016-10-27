#pragma once

#include <condition_variable>
#include <map>
#include <memory>
#include <mutex>
#include <thread>

class IProcess;

class ProcessManager
{
public:
	ProcessManager() = default;
	ProcessManager(const ProcessManager& o) = delete;
	ProcessManager(ProcessManager&& o) = default;

	~ProcessManager();

	ProcessManager& operator=(const ProcessManager& o) = delete;
	ProcessManager& operator=(ProcessManager&& o) = default;

	std::shared_ptr<IProcess> add(std::unique_ptr<IProcess> process);

	void shutdown() noexcept;

	void wait() noexcept;

private:
	void threadProc(IProcess& process);

private:
	std::condition_variable mFinished;
	std::mutex mMutex;
	std::map<std::thread::id, std::shared_ptr<IProcess>> mActive;
};

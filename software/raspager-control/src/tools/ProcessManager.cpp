#include "ProcessManager.h"
#include "IProcess.h"
#include <stdexcept>

using namespace std;

ProcessManager::~ProcessManager()
{
	shutdown();
}

shared_ptr<IProcess> ProcessManager::add(unique_ptr<IProcess> process)
{
	if (!process)
	{
		throw invalid_argument("Process is null.");
	}

	auto sp = shared_ptr<IProcess>(move(process));
	auto t = thread(&ProcessManager::threadProc, this, std::ref(*sp));

	try
	{
		unique_lock<mutex> lck(mMutex);
		mActive.emplace(t.get_id(), sp);
		lck.unlock();

		t.detach();

		return sp;
	}
	catch (...)
	{
		sp->terminate();
		t.join();

		throw;
	}
}

void ProcessManager::shutdown() noexcept
{
	lock_guard<mutex> lck(mMutex);

	for (const auto& p : mActive)
	{
		p.second->terminate();
	}
}

void ProcessManager::wait() noexcept
{
	unique_lock<mutex> lck(mMutex);
	if (!mActive.empty())
	{
		mFinished.wait(lck);
	}
}

void ProcessManager::threadProc(IProcess& process)
{
	process.run();

	unique_lock<mutex> lck(mMutex);
	mActive.erase(this_thread::get_id());
	lck.unlock();

	mFinished.notify_all();
}

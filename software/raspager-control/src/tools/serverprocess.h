#pragma once

#include "IProcess.h"
#include "raspagerdigiextension.h"
#include <atomic>
#include <memory>
#include <string>


class ServerProcess : public IProcess
{
public:
	ServerProcess(RaspagerDigiExtension& extension, int port);

	ServerProcess(const ServerProcess& o) = delete;

	~ServerProcess();

	ServerProcess& operator=(const ServerProcess& o) = delete;

	virtual void run() noexcept override;

	virtual void terminate() noexcept override;

private:
	std::string getTextData();
	void acceptClient();

private:
	std::atomic_bool mKill;
	int mSD;
	int mPort;
	RaspagerDigiExtension* mExtension;
};

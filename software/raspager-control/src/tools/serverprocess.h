#pragma once

#include <atomic>
#include <memory>
#include <string>
#include "../tools/raspagerdigiextension.h"


class ServerProcess
{
public:
	ServerProcess(RaspagerDigiExtension* myExtension);

	ServerProcess(const ServerProcess& o) = delete;

	~ServerProcess();

	ServerProcess& operator=(const ServerProcess& o) = delete;

	void run(int port);

	void stop();
	
	RaspagerDigiExtension* myRaspagerDigiExtension;

private:
	std::string getTextData();
	void acceptClient();

private:
	std::atomic_bool mKill;
	int mSD;
};

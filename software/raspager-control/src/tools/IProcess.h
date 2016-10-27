#pragma once

class IProcess
{
public:
	virtual ~IProcess() = default;

	virtual void run() noexcept = 0;

	virtual void terminate() noexcept = 0;
};

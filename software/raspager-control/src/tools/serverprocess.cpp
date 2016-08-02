#include "serverprocess.h"
#include <cstring>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

ServerProcess::ServerProcess(RaspagerDigiExtension* myExtension) :
	mKill(false),
	mSD(-1)
{
	this->myRaspagerDigiExtension = myExtension;
}


ServerProcess::~ServerProcess()
{
	if (mSD > 0)
	{
		close(mSD);
	}
}

void ServerProcess::run(int port)
{
	if (mSD != -1)
	{
		cout << "Server already running.\n";
		return;
	}

	if (mKill)
	{
		return;
	}

	cout << "Starting server process.\n";

	mSD = socket(AF_INET, SOCK_STREAM, 0);
	if (mSD == -1)
	{
		throw runtime_error("Failed to create server socket.");
	}

	const int yes = 1;
	setsockopt(mSD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	if (bind(mSD, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1)
	{
		close(mSD);
		throw runtime_error("Failed to bind server socket.");
	}

	if (listen(mSD, 10) == -1)
	{
		close(mSD);
		throw runtime_error("Failed to listen on server socket.");
	}

	cout << "Server started.\n";

	while (!mKill)
	{
		acceptClient();
	}

	cout << "Server stopped.\n";
}

void ServerProcess::stop()
{
	mKill = true;
}

string ServerProcess::getTextData()
{
/*	SensorData data;
	mReader->read(data);

	ostringstream text;
	text << fixed << setfill('0') << setprecision(2) << data.UBat <<
		";" << data.IBat << ";" << data.UPsu << ";" << data.IPsu;
*/
	ostringstream text;
	double dVoltage = this->myRaspagerDigiExtension->readVoltage();
	text << "Voltage" << dVoltage << "\n";
	return text.str();
}

void ServerProcess::acceptClient()
{
	sockaddr addr;
	socklen_t addrlen = sizeof(addr);

	int fd = accept(mSD, &addr, &addrlen);
	if (fd == -1)
	{
		if (errno != EINTR)
		{
			cout << "Failed to accept connection.";
		}

		return;
	}

	cout << "Client connected.\n";

	try
	{
		const auto txt = getTextData();
		const auto len = write(fd, txt.c_str(), txt.size());
		if (len == -1)
		{
			cout << "Failed to write data.";
		}
	}
	catch (const exception& ex)
	{
//		mLogger->log(LogLevel::ERROR, ex.what());
	}

	close(fd);

	cout << "Client connection closed.\n";
}

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

ServerProcess::ServerProcess(RaspagerDigiExtension& extension, int port) :
	mKill(false),
	mSD(-1),
	mPort(port),
	mExtension(&extension)
{
}


ServerProcess::~ServerProcess()
{
	if (mSD != -1)
	{
		close(mSD);
	}
}

void ServerProcess::run() noexcept
{
	if (mSD != -1)
	{
		cerr << "Server already running." << endl;
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
		//throw runtime_error("Failed to create server socket.");
		cerr << "Failed to create server socket: " << strerror(errno) << endl;
	}

	const int yes = 1;
	setsockopt(mSD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(mPort);

	if (bind(mSD, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1)
	{
		cerr << "Failed to bind server socket: " << strerror(errno) << endl;
		close(mSD);
		//throw runtime_error("Failed to bind server socket.");
	}

	if (listen(mSD, 10) == -1)
	{
		cerr << "Failed to listen on server socket: " << strerror(errno) << endl;
		close(mSD);
		//throw runtime_error("Failed to listen on server socket.");
	}

	cout << "Server started.\n";

	while (!mKill)
	{
		acceptClient();
	}

	cout << "Server stopped.\n";
}

void ServerProcess::terminate() noexcept
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
	double dVoltage = this->mExtension->readVoltage();
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
			cerr << "Failed to accept connection: " << strerror(errno) << endl;
		}

		return;
	}

	cout << "Client connected." << endl;

	try
	{
		const auto txt = getTextData();
		const auto len = write(fd, txt.c_str(), txt.size());
		if (len == -1)
		{
			cerr << "Failed to write data: " << strerror(errno) << endl;
		}
	}
	catch (const exception& ex)
	{
		cerr << "AcceptClient: " << ex.what() << endl;
		// mLogger->log(LogLevel::ERROR, ex.what());
	}

	close(fd);

	cout << "Client connection closed." << endl;
}

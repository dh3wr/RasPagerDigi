#pragma once

#include "server_ws.hpp"
#include "json_spirit.h"
#include <iostream>
#include <string>

#define WSServerPort 	8080

struct wsdata {
	double fwdpwr;
	double revpwr;
	double swr;
	double voltage;
	double current;
	double fwdpwrmean;
	double revpwrmean;
	double swrmean;
};

struct wstemp {
	double INTemp;
	double OUTTemp;
	double APRSTemp;
	double PATemp;
	double ExtTemp1;
	double ExtTemp2;
	double ExtTemp3;
	double ExtTemp4;
};

class WebSocketServer {
using WsServer = SimpleWeb::SocketServer<SimpleWeb::WS>;

private:
	WsServer server;
	RaspagerDigiExtension& myExtension;


public:
	WebSocketServer(RaspagerDigiExtension& myExtension);
	~WebSocketServer();
	void run();
	bool SendData(const wsdata& data);
	bool SendTemp(const wstemp& temp);

private:
	void OnMessage(std::shared_ptr<WsServer::Connection> connection, std::shared_ptr<WsServer::Message> message);
};

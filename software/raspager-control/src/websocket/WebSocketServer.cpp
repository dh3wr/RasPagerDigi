#include "WebSocketServer.h"
#include "../tools/raspagerdigiextension.h"
#include <functional>

using namespace std;

WebSocketServer::WebSocketServer(RaspagerDigiExtension& myExtension) :
	myExtension(myExtension)
{
	// From define
    this->server.config.port = WSServerPort;

    auto& echo_all = this->server.endpoint["^/echo_all/?$"];
    echo_all.on_message = std::bind(&WebSocketServer::OnMessage, this, placeholders::_1, placeholders::_2);
}

WebSocketServer::~WebSocketServer() {
	//if (server_thread.joinable()) {
	//	server_thread.join();
	//}
}

void WebSocketServer::run() {
	//    thread server_thread([this->&server](){
    //    //Start WS-server
    //    this->server.start();
    //});

    //Wait for server to start so that the client can connect
    //this_thread::sleep_for(chrono::seconds(1));
    server.start();
}

bool WebSocketServer::SendData(const wsdata& data) {
	json_spirit::Object addr_obj;
	addr_obj.push_back( json_spirit::Pair( "Voltage", round(data.voltage * 10.0) / 10.0 ) );
	addr_obj.push_back( json_spirit::Pair( "Current", round(mywsdata.current * 10.0) / 10.0 ) );
	addr_obj.push_back( json_spirit::Pair( "PowerForward", round(data.fwdpwr * 10.0) / 10.0 ) );
	addr_obj.push_back( json_spirit::Pair( "PowerForwardLastTX", round(data.fwdpwrmean * 10.0) / 10.0 ) );
	addr_obj.push_back( json_spirit::Pair( "PowerReflect", round(data.revpwr * 10.0) / 10.0 ) );
	addr_obj.push_back( json_spirit::Pair( "PowerReflectLastTX", round(data.revpwrmean * 10.0) / 10.0 ) );
	addr_obj.push_back( json_spirit::Pair( "PowerVSWR", round(data.swr * 100.0) / 100.0 ) );
	addr_obj.push_back( json_spirit::Pair( "PowerVSWRLastTX", round(data.swrmean * 100.0) / 100.0 ) );
	addr_obj.push_back( json_spirit::Pair( "Slots", "1283ABC" ) );

	std::string testinfo = json_spirit::write_formatted(addr_obj);

//	auto testinfo = "Das ist ja toll";
	//echo_all.get_connections() can also be used to solely receive connections on this endpoint
    for(auto& a_connection: this->server.get_connections()) {
        auto send_stream=make_shared<WsServer::SendStream>();
        *send_stream << testinfo;

        //server.send is an asynchronous function
        this->server.send(a_connection, send_stream);
	}

	return true;
}

bool WebSocketServer::SendTemp( const wstemp& temp)
{
	json_spirit::Object addr_obj;
	addr_obj.push_back( json_spirit::Pair( "INTemp", round(temp.INTemp * 10.0) / 10.0 ) );
	addr_obj.push_back( json_spirit::Pair( "OUTTemp", round(temp.OUTTemp * 10.0) / 10.0 ) );
	addr_obj.push_back( json_spirit::Pair( "APRSTemp", round(temp.APRSTemp * 10.0) / 10.0 ) );
	addr_obj.push_back( json_spirit::Pair( "PATemp", round(temp.PATemp * 10.0) / 10.0 ) );
	addr_obj.push_back( json_spirit::Pair( "ExtTemp1", round(temp.ExtTemp1 * 10.0) / 10.0 ) );
	addr_obj.push_back( json_spirit::Pair( "ExtTemp2", round(temp.ExtTemp2 * 10.0) / 10.0 ) );
	addr_obj.push_back( json_spirit::Pair( "ExtTemp3", round(temp.ExtTemp3 * 10.0) / 10.0 ) );
	addr_obj.push_back( json_spirit::Pair( "ExtTemp4", round(temp.ExtTemp4 * 10.0) / 10.0 ) );

	std::string testinfo = json_spirit::write_formatted(addr_obj);

//	auto testinfo = "Das ist ja toll";
	//echo_all.get_connections() can also be used to solely receive connections on this endpoint
    for(auto& a_connection: this->server.get_connections()) {
        auto send_stream=make_shared<WsServer::SendStream>();
        *send_stream << testinfo;

        //server.send is an asynchronous function
        this->server.send(a_connection, send_stream);
	}

	return true;
}

void WebSocketServer::OnMessage(shared_ptr<WsServer::Connection> connection,
	shared_ptr<WsServer::Message> message)
{
	auto message_str=message->string();
	//		cout << message_str;

	double SetPowerReal = 0.0;
	json_spirit::Value val;

	auto success = json_spirit::read(message_str, val);
	if (success) {
		auto jsonObject = val.get_obj();

		for (const auto& entry : jsonObject) {
			if (entry.name_ == "SetPower" && entry.value_.type() == json_spirit::Value_type::real_type) {
				SetPowerReal = entry.value_.get_real();
				break;
			}
		}
	}

	cout << "Power set to: " << SetPowerReal << endl;

	myExtension->setOutputPower_Watt(SetPowerReal);
}

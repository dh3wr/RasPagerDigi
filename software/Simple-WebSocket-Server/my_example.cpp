#include "server_ws.hpp"
#include "client_ws.hpp"
#include "json_spirit.h"
#include <iostream>
#include <string>


using namespace std;

typedef SimpleWeb::SocketServer<SimpleWeb::WS> WsServer;

int main() {
    //WebSocket (WS)-server at port 8080 using 1 thread
    WsServer server;
    server.config.port=8080;

    //Example 3: Echo to all WebSocket endpoints
    //  Sending received messages to all connected clients
    //  Test with the following JavaScript on more than one browser windows:
    //    var ws=new WebSocket("ws://localhost:8080/echo_all");
    //    ws.onmessage=function(evt){console.log(evt.data);};
    //    ws.send("test");
    auto& echo_all=server.endpoint["^/echo_all/?$"];
    echo_all.on_message=[&server](shared_ptr<WsServer::Connection> /*connection*/, shared_ptr<WsServer::Message> message) {
        auto message_str=message->string();
        
        //echo_all.get_connections() can also be used to solely receive connections on this endpoint
        for(auto a_connection: server.get_connections()) {
            auto send_stream=make_shared<WsServer::SendStream>();
            *send_stream << message_str;
            
            //server.send is an asynchronous function
            server.send(a_connection, send_stream);
        }
    };
    
    thread server_thread([&server](){
        //Start WS-server
        server.start();
    });
    
    //Wait for server to start so that the client can connect
    this_thread::sleep_for(chrono::seconds(1));

	
	json_spirit::Object addr_obj;

	addr_obj.push_back( json_spirit::Pair( "Voltage", 12.34 ) );
	addr_obj.push_back( json_spirit::Pair( "Current", 9.87 ) );
	addr_obj.push_back( json_spirit::Pair( "PowerForward", 12.3 ) );
	addr_obj.push_back( json_spirit::Pair( "PowerForwardLastTX", 10.3 ) );
	addr_obj.push_back( json_spirit::Pair( "PowerReflect", 1.3 ) );
	addr_obj.push_back( json_spirit::Pair( "PowerReflectLastTX", 2.0 ) );
	addr_obj.push_back( json_spirit::Pair( "PowerVSWR", 1.3 ) );
	addr_obj.push_back( json_spirit::Pair( "PowerVSWRLastTX", 1.4 ) );
	addr_obj.push_back( json_spirit::Pair( "Slots", "1283ABC" ) );

	
	std::string testinfo = json_spirit::write_formatted(addr_obj);
	
//	auto testinfo = "Das ist ja toll";
	while (1) {
		sleep(1);
		//echo_all.get_connections() can also be used to solely receive connections on this endpoint
        for(auto a_connection: server.get_connections()) {
            auto send_stream=make_shared<WsServer::SendStream>();
            *send_stream << testinfo;
            
            //server.send is an asynchronous function
            server.send(a_connection, send_stream);
			
			
        }
	}
   
    server_thread.join();

    
    return 0;
}

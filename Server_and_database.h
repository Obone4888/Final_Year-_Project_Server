#pragma once
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

using namespace std;

class functions
{
	public:
		SOCKET start_server(SOCKET listening);
		void connect_client(SOCKET listening);
		static int callback(void* data, int argc, char** argv, char** ColName);
		void SQL_Query(string userQuery);
		void firewall(string Packet);
};

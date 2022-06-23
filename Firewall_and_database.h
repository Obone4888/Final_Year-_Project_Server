#pragma once
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

using namespace std;

class fn_data
{
public:
	
	static int callback(void* data, int argc, char** argv, char** ColName);
	string SQL_Query(string userQuery);

};


#include <iostream>
#include <string>
#include <sqlite3.h>
#include"Server_and_client.h"
#include"Firewall_and_database.h"
#include <WS2tcpip.h>
#include <tuple>
#include "winsock2.h"
#include <vector>
#pragma comment (lib, "ws2_32.lib")
using namespace std;


const string Rules[12] = { "SELECT Country_Name FROM Country_population;", "SELECT Country_Name, Population FROM	Country_population;",
							"SELECT Country_Name, Predicted_population FROM	Country_population;","SELECT Country_Name, Population, Predicted_population FROM	Country_population;",
							"SELECT ID, Country_Name FROM Country_population;" };
bool FirewallAccepted = false;
string querymain;
fn_data fn; 
SOCKET functions::start_server(SOCKET listening)
{
	if (listening == INVALID_SOCKET)
	{
		cerr << "can't create socket" << endl;
		return 0;
	}
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = ADDR_ANY;
	bind(listening, (sockaddr*)&hint, sizeof(hint));

	listen(listening, SOMAXCONN);
	return listening;
}

void functions::connect_client(SOCKET listening)
{
	string userRequest;
	sockaddr_in client;
	int clientsize = sizeof(client);
	
	SOCKET clientsocket;
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];
	char buf[4096];
	ZeroMemory(buf, 4096);
	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << "connected on port" << service << endl;
	}
	else
	{
		//connects to server 
		clientsocket =  accept(listening, (sockaddr*)&client, &clientsize);
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port: " << ntohs(client.sin_port) << endl;
	}
	
	while (true)
	{		
		int bytesRecived = recv(clientsocket, buf, 4096, 0);
		if (bytesRecived == SOCKET_ERROR)
		{
			cerr << "error in recv" << endl;
			break;
		}
		if (bytesRecived == 0)
		{
			cout << "client disconntected" << endl;
			break;
		}
		if(bytesRecived >  0)
		{		 	
			firewall(buf);			
		}
		
		if (FirewallAccepted == true)
		{			
			querymain += fn.SQL_Query(buf);
			char* returnquery = const_cast<char*> (querymain.c_str());
			send(clientsocket, returnquery, strlen(returnquery), 0);
			returnquery = NULL;
			querymain = "";
			FirewallAccepted = false;
		}
		else
		{
			char* returnquery = const_cast<char*> (querymain.c_str());
			send(clientsocket, returnquery, strlen(returnquery), 0);
			querymain = "";		
			FirewallAccepted = false;
		}	
		
	}
	
	closesocket(clientsocket);
}
void functions::firewall(string Packet)
{

	for (int i = 0; i < sizeof(Rules); i++)
	{
		if (Rules[i] == Packet && Rules[i] != "")
		{
			FirewallAccepted = true;
			break;
		}
	}
	if (FirewallAccepted == true)
	{
		querymain += " your packet was accepted\r\n";
		cout << "packet was accepted\n" << endl;;
	}
	else if (FirewallAccepted == false)
	{
		querymain += " your packet was not accepted\r\n";
		cout << "packet was not accepted\n" << endl;
	}

	
}




#include"pch.h"
#include <WinSock2.h>
#include <iostream>
#include <WS2tcpip.h>

#include "server.h"

#pragma comment(lib, "Ws2_32.lib")

int main()
{

	std::cout << "Wcisnij 1 zeby byc serwerem " << endl;

	int input;
	do
	{
		std::cin >> input;
		if (input == 1)
		{
			cout << "SERWER " << endl;
			server s;
			s.TCP();
		}
	
	} while (input != 1 || input != 2);
}
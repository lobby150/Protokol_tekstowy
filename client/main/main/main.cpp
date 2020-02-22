#include"pch.h"
#include <WinSock2.h>
#include <iostream>
#include <WS2tcpip.h>


#include "client.h"
#pragma comment(lib, "Ws2_32.lib")

int main()
{

	std::cout << "Wcisnij 1 zeby byc klientem " << endl;


	int input;
	do
	{
		std::cin >> input;
		if (input == 1)
		{
			cout << "KLIENT " << endl;
			client s;
			s.main();
		}
		
	} while (input != 1 );
}
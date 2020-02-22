#pragma once
#include <WinSock2.h>
#include <iostream>
#include <WS2tcpip.h>
#include <thread>
#include <chrono>
#include <string>
#include <ctime>
#include"pakiet.h"
#pragma comment(lib, "Ws2_32.lib")

class client
{
private:
	const int rozmiar = 128;
	char dane[128];


	const int rozmiar2 = 25;
	char dane2[25];
	WSADATA wsaData;
	SOCKET s;
	SOCKADDR_IN dest_addr;
	Pakiet pakiet;
	int input;
	int ilosc_prob;
	int counter = 0;
	void zadanie(SOCKET klient);
	void pakiet_zero();
	void pakowanie_danych();
	void wyluskanie_danych();
	void przeslanie_znacznika(SOCKET s);
	void wyluskanie_znacznika();
	void wyslij_raz(SOCKET s);
	void wyslij_dwa(SOCKET s);
	void zeruj_buffer();
	void przetworz(const string &bufor);
	void odbierz(SOCKET s);
	void wyslij_w(SOCKET s);
public:
	void main();
};

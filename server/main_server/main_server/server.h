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
using namespace std;

class server
{
public:
	WSADATA wsaData;
	SOCKADDR_IN my_address, current_address;
	SOCKET ListenSocket, AcceptSocket;
	int sin_size = sizeof(sockaddr_in);
	SOCKET klienci[2];
	std::thread tablica_watkow[2];
	int i;
	Pakiet pakiet;
	const int rozmiar = 128;
	char dane[128];

	const int rozmiar2 = 25;
	char dane2[25] = "";

	int LOSOWA_LICZBA;
	int ilosc_prob = 0;
	bool wygrany_mecz = false;
	int counter = 0;
	void zadanie(SOCKET klient);
	void pakiet_zero();
	void pakowanie_danych();
	void wyluskanie_danych();
	void TCP();
	int wylosowany_identyfikator;
	int zajety_identyfikator;
	void przeslanie_znacznika(SOCKET s);
	void wyluskanie_znacznika();
	void wyslij_raz(SOCKET s);
		void wyslij_dwa(SOCKET s);
		void zeruj_buffer();
		void przetworz(const string &bufor);
		void odbierz(SOCKET s);
		void wyslij_w(SOCKET s);
};

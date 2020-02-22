#include "pch.h"
#include "server.h"
#include"pakiet.h"
#include<time.h>

#pragma warning(disable:4996)


long int unix_timestamp()
{
	time_t t = std::time(0);
	long int now = static_cast<long int> (t);
	return now;
}

int odpowiedz = 0;
int ilosc_graczy = 0;
void server::zadanie(SOCKET klient)
{
	static int counter = 0;
	pakiet.znacznik_czasu = to_string(unix_timestamp());
	odbierz(klient);


	


	counter++;
	while (counter != 2)
	{
		std::cout << "Oczekiwanie " << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}

	if (counter == 2) // [2]
	{
		//przeslanie_znacznika(klient);
		pakiet_zero();
	
		pakiet.operacja = "pj";
		
		pakiet.znacznik_czasu = to_string(unix_timestamp());
		
		pakiet.odpowiedz = 99;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
		wyslij_w(klient);

		for (int i = 0; i < 99; i++)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

			odbierz(klient);
			
		
			


			if (pakiet.odpowiedz == LOSOWA_LICZBA)
			{
			//	przeslanie_znacznika(klient);
				pakiet_zero();
				
				pakiet.operacja = "wa";
				pakiet.odpowiedz = LOSOWA_LICZBA;
				pakiet.znacznik_czasu = to_string(unix_timestamp());
				
				
				wygrany_mecz = true;
	
				wyslij_w(klient);
				
				if (recv(klient, dane, rozmiar, 0) == 0)
				{
					closesocket(klient);
				}
				break;
			}

			odpowiedz++;

			while (odpowiedz % 2 != 0) // [2] czekanie az drugi gracz tez wysle wiadomosc, chyba ze ktos wygral
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}

			if (wygrany_mecz == true)
			{
			//	przeslanie_znacznika(klient);
				pakiet_zero();
			
				pakiet.operacja = "pw";
				pakiet.odpowiedz = LOSOWA_LICZBA;
				pakiet.znacznik_czasu = to_string(unix_timestamp());
				
				wyslij_w(klient);
				
				if (recv(klient, dane, rozmiar, 0) == 0)
				{
					closesocket(klient);
				}

				break;
			}
			else if (i + 1 == 99)
			{
			//		przeslanie_znacznika(klient);
				std::cout << "PORAZKA " << std::endl;
				pakiet_zero();
				
				pakiet.operacja = "pa";
				pakiet.odpowiedz = LOSOWA_LICZBA;
				pakiet.znacznik_czasu = to_string(unix_timestamp());
		
			
				wyslij_w(klient);
			
				if (recv(klient, dane, rozmiar, 0) == 0)
				{
					closesocket(klient);
				}
				break;
			}
			else
			{
			//	przeslanie_znacznika(klient);
				pakiet_zero();
				
				pakiet.operacja = "nz";
				pakiet.odpowiedz = LOSOWA_LICZBA;
				pakiet.znacznik_czasu = std::to_string(unix_timestamp());
			

				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				
				wyslij_w(klient);
			

			}



		}

	}
}

void server::TCP()
{
	/* Inicjalizacja losowosci */
	srand(time(NULL));
	LOSOWA_LICZBA = 26;

	/* Inicjalizacja WinSock */
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	/* Inicjalizacja pól - rodzina adresow, numer portu, adres IP */
	my_address.sin_family = AF_INET; //IPv4
	my_address.sin_port = htons(25735);
	inet_pton(AF_INET, "127.0.0.1", &my_address.sin_addr);

	/* tworzenie socketow - nasluchujacego oraz akceptujacego polaczenie */
	ListenSocket, AcceptSocket = INVALID_SOCKET;
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	/* przypisanie socketa na podany port*/
	if (::bind(ListenSocket, (SOCKADDR *)& my_address, sin_size) == SOCKET_ERROR)
	{

		printf("bind() failed.\n");
		closesocket(ListenSocket);

	}
	memset(&current_address, 0, sizeof(current_address));
	inet_pton(AF_INET, "127.0.0.1", &current_address.sin_addr);
	current_address.sin_port = htons(25735);
	/* nasluchiwanie na podanym porcie (2 mozliwe polaczenia w kolejce)*/
	if (listen(ListenSocket, 2) == SOCKET_ERROR)
	{
		cout << "LISTEN PROBLEM " << endl;
	}
	for (;;)
	{
		/* jezeli ktos sie polaczy, to przypisujemy go do AcceptSocket*/
		AcceptSocket = accept(ListenSocket, (SOCKADDR *)& current_address, &sin_size);
		if (AcceptSocket != INVALID_SOCKET)
		{
			/*witanie na serwerze*/
			//przeslanie_znacznika(AcceptSocket);
			pakiet_zero();
			
			pakiet.operacja = "id";
			wylosowany_identyfikator = rand() % 89 + 10;
			while (zajety_identyfikator == wylosowany_identyfikator)
			{
				wylosowany_identyfikator = rand() % 89 + 10;
			}
			zajety_identyfikator = wylosowany_identyfikator;
			pakiet.id = wylosowany_identyfikator;
		

			pakiet.znacznik_czasu = to_string(unix_timestamp());
			
			
			wyslij_w(AcceptSocket);
			


			/* odbior wiadomosci od klienta */

			odbierz(AcceptSocket);
			

			if (pakiet.operacja == "zd"&& ilosc_graczy < 2) // przyjecie gracza
			{
				klienci[ilosc_graczy] = AcceptSocket;
				//send(AcceptSocket, dane, rozmiar, 0);
				pakiet_zero();
				
				pakiet.operacja = "pj";
				pakiet.znacznik_czasu = std::to_string(unix_timestamp());
				
				//przeslanie_znacznika(AcceptSocket);
				wyslij_w(AcceptSocket);
			
				tablica_watkow[ilosc_graczy] = std::thread(&server::zadanie, this, AcceptSocket);
				if (ilosc_graczy == 1)
				{

					tablica_watkow[0].detach();
					tablica_watkow[1].detach();
				}


				ilosc_graczy++;


			}
			else if ( pakiet.operacja == "zd" && ilosc_graczy == 2) // brak miejsca
			{
				//send(AcceptSocket, dane, rozmiar, 0);
				pakiet_zero();
			
				pakiet.operacja = "bm";
				pakiet.znacznik_czasu = std::to_string(unix_timestamp());
				wyslij_w(AcceptSocket);
				//przeslanie_znacznika(AcceptSocket);
			
		
		
				closesocket(AcceptSocket);
			}
			else // error
			{
				pakiet_zero();
				
				pakiet.operacja = "er";
				pakiet.znacznik_czasu = std::to_string(unix_timestamp());
				std::cout << "\nNie marnuj na niego czasu" << std::endl;
				if (recv(AcceptSocket, dane, rozmiar, 0) == 0)
				{
					closesocket(AcceptSocket);
				}
			}
			AcceptSocket = INVALID_SOCKET;

		}
	}
}

/*void server::wyluskanie_danych()
{
	std::string pom;
	string x = "";
	// to mozna w petli for
	for (int i = 0; i < 61; i++)
	{
		x = dane[i];
		pom += x;
	}

	pakiet.operacja = pom.substr(9, 2);

	pakiet.odpowiedz = stoi(pom.substr(22, 2));
	pakiet.id = stoi(pom.substr(28, 2));
	pakiet.NS = pom.substr(34, 1);
	pakiet.znacznik_czasu = stoi(pom.substr(51, 10));
	cout << pom << endl;
}*/

void server::wyslij_raz(SOCKET s)
{
	string pom;
	zeruj_buffer();
	pakiet.NS = to_string(2);
	pom = "Operacja=";
	pom += pakiet.operacja;
	pom += ";ID=";
	pom += to_string(pakiet.id);
	pom += ";NSekwencyjny=";
	pom += pakiet.NS;
	
	pom += ";Znacznik_czasu=";
	pom += pakiet.znacznik_czasu;
	pom += ";";
	
	for (int i = 0; i < pom.length(); i++)
	{
		dane[i] = pom[i];
	}
	send(s, dane, pom.size(), 0);

	cout << pom << endl;
}
void server::wyslij_dwa(SOCKET s)
{
	string pom;
	zeruj_buffer();
	pakiet.NS = to_string(1);
	pom = "Odpowiedz=";
	pom += to_string(pakiet.odpowiedz);
	pom += ";ID=";
	pom += to_string(pakiet.id);
	pom += ";NSekwencyjny=";
	pom += pakiet.NS;

	pom += ";Znacznik_czasu=";
	pom += pakiet.znacznik_czasu;
	pom += ";";

	for (int i = 0; i < pom.length(); i++)
	{
		dane[i] = pom[i];
	}
	send(s, dane, pom.size(), 0);

	cout << pom << endl;
 }
void server::wyslij_w(SOCKET s)
{
	wyslij_raz(s);
	wyslij_dwa(s);
}
void server::przetworz(const string &bufor)
{
	if (bufor.find("Operacja=") < 128)
	{
		counter = bufor.find("Operacja=");
		pakiet.operacja = bufor.substr(counter + 9, 2);
	
		counter = 0;
	}
	else if (bufor.find("Odpowiedz=") < 128)
	{
		counter = bufor.find("Odpowiedz=");
		pakiet.odpowiedz = stoi(bufor.substr(counter + 10, 2));
		
		counter = 0;
	
	}
	
}
void server::odbierz(SOCKET s)
{
	recv(s, dane, sizeof(dane), 0);
	przetworz(dane);
	recv(s, dane, sizeof(dane), 0);
	przetworz(dane);


}
void server::pakiet_zero()
{
	pakiet.operacja = "";
	
	pakiet.odpowiedz = 0;
	pakiet.id = 0;
	pakiet.NS = "3";
	pakiet.znacznik_czasu = to_string(unix_timestamp());
}
void server::zeruj_buffer()
{
	for (int i = 0; i < 128; i++)
	{
		dane[i] = '\0';
	}
}
/*void server::przeslanie_znacznika(SOCKET s)
{
	string pom;
	pom = "Znacznik_czasu_komunikat=";
	pom += to_string(unix_timestamp());

	char dane3[35];
	pom.copy(dane3, 35);

	for (int i = 0; i < 35; i++)
	{
		dane[i] = dane3[i];
	}
	cout << pom << endl;

	send(s, dane, rozmiar, 0);
}
void server::wyluskanie_znacznika()
{
	string pom;
	string x = "";
	for (int i = 0; i < 25; i++)
	{
		x = dane2[i];
		pom += x;
	}
	pakiet.znacznik_czasu = pom.substr(15, 10);
	cout << pom << endl;
}*/

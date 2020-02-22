#include "pch.h"
#include "client.h"
#include<time.h>
#include<Windows.h>
#pragma warning(disable:4996)

#pragma warning(disable:4996)


long int unix_timestamp()
{
    time_t t = std::time(0);
    long int now = static_cast<long int> (t);
    return now;
}


void client::main()
{
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(25735);
	inet_pton(AF_INET, "127.0.0.1", &dest_addr.sin_addr);

	connect(s, (SOCKADDR*)& dest_addr, sizeof(sockaddr));
	/* odbiór pierwszego pakietu, który zawiera powitanie oraz identyfikator, ktory jest w polu LICZBA */
	odbierz(s);

	
	
	


	/* przypisanie LICZBY jako ID*/

	std::cout << "\n1. proba dolaczenia do serwera gry\n2.rozlaczenie\n" << std::endl;

	std::cin >> input;
	if (input == 1)
	{
		//przeslanie_znacznika(s);
		pakiet_zero();
	
		pakiet.operacja = "zd";
		pakiet.znacznik_czasu = std::to_string(unix_timestamp());
		wyslij_w(s);
		
		odbierz(s);

		cout << pakiet.operacja << endl;
	
	
		if ( pakiet.operacja == "pj")
		{

			//przeslanie_znacznika(s);
			pakiet_zero();
			
			pakiet.operacja = "wl";
		
			pakiet.znacznik_czasu = std::to_string(unix_timestamp());
			wyslij_w(s);
		
		

		
		
			std::cout << "poczekaj za drugim graczem " << std::endl;

			odbierz(s);
		
			//wyluskanie_danych();
			

			cout << "Mozna zaczac zgadywanie " << endl;
			for (int i = 0; i < 99; i++)
			{



				cout << "Podaj liczbe z zakresu 10;99" << endl; std::cin >> input;
				while (input < 10 || input>99)
				{
					cout << "Znowu zle, patrz zakres wyzej... " << endl;

					cin >> input;
				}
			//	przeslanie_znacznika(s);
				pakiet_zero();
			
				pakiet.operacja = "wz";
				pakiet.znacznik_czasu = std::to_string(unix_timestamp());
				pakiet.odpowiedz = input;
				wyslij_w(s);
			
			
				

				std::cout << "\nOczekiwanie na ruch drugiego gracza" << std::endl;

				odbierz(s);
				
				
				
				if ( pakiet.operacja == "wa")
				{
					std::cout << "Brawo zgadles!" << std::endl;
					Sleep(5000);
					closesocket(s);
					i = ilosc_prob - 1;
					break;
				}
				else if ( pakiet.operacja == "pa")
				{
					std::cout << "Niestety oboje przegraliscie mecz, poprawna odpowiedz to: " << pakiet.odpowiedz;
					closesocket(s);
					i = ilosc_prob - 1;
					break;
				}
				else if ( pakiet.operacja == "pw")
				{
					std::cout << "Niestety Twoj przeciwnik byl lepszy (RNG MASTER), poprawna odpowiedz to: " << pakiet.odpowiedz;
					closesocket(s);
					i = ilosc_prob - 1;
					break;
				}
				else
				{
					std::cout << "PUDLO! Probuj dalej!" << std::endl;
				}
			}
		}
		else if ( pakiet.operacja == "bm")
		{
			std::cout << "Na serwerze nie ma miejsca" << std::endl;
		}
		else
		{
			std::cout << "Wyst¹pi³ nieznany b³¹d" << std::endl;
		}
	}
	else
	{
	//przeslanie_znacznika(s);
		pakiet_zero();
		
		pakiet.operacja = "er";
		pakiet.znacznik_czasu = std::to_string(unix_timestamp());
		shutdown(s, 2);
	
		wyslij_w(s);
	
		std::cout << "Roz³¹czam siê.";
		closesocket(s);
	}
}

/*void client::wyluskanie_danych()
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
}

void client::pakowanie_danych()
{
	string pom;
	pom = "Operacja=";
	pom += pakiet.operacja;


	pom += ";Odpowiedz=";
	pom += to_string(pakiet.odpowiedz);
	pom += ";ID=";
	pom += to_string(pakiet.id);
	pom += ";NS=";
	pom += pakiet.NS;
	pom += ";Znacznik_czasu=";
	pom += to_string(unix_timestamp());

	char dane1[61];
	pom.copy(dane1, 61);

	for (int i = 0; i < 61; i++)
	{
		dane[i] = dane1[i];
	}


	cout << pom << endl;
}
*/


void client::pakiet_zero()
{
	pakiet.operacja = "";

	pakiet.odpowiedz =0;
	
;	pakiet.NS = "3";
	pakiet.id = 0;
	pakiet.znacznik_czasu = to_string(unix_timestamp());
}
void client::wyslij_raz(SOCKET s)
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
void client::wyslij_dwa(SOCKET s)
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
void client::wyslij_w(SOCKET s)
{
	wyslij_raz(s);
	wyslij_dwa(s);
}
void client::przetworz(const string &bufor)
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
void client::odbierz(SOCKET s)
{
	recv(s, dane, sizeof(dane), 0);
	przetworz(dane);
	recv(s, dane, sizeof(dane), 0);
	przetworz(dane);

	
}

void client::zeruj_buffer()
{
	for (int i = 0; i < 128; i++)
	{
		dane[i] = '\0';
	}
}


/*
void client::przeslanie_znacznika(SOCKET s)
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
void client::wyluskanie_znacznika()
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


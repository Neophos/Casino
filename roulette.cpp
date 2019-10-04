// Alla includes
#include <iostream>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <algorithm>
#include "roulette.h"

// Ta in och hantera data fr�n spelaren
void Roulette::GetUserInput()
{
	// Variabler som beh�vs i funktionen
	std::wstring input;
	bool acceptableInput = false;
	std::variant<int, std::wstring> bet;
	int wager = 0;

	do {
		// Str�ngar m�ste prefixas med L f�r att vara breda s� de kan s�ttas ihop korrekt
		std::wcout << L"Du har " + player.PrintMoney() + L" kronor kvar att spela med." << std::endl;
		std::wcout << "F�ljande vad �r accepterade:" << std::endl;
		std::wcout << "R�d, red, r: Satsa p� r�d." << std::endl;
		std::wcout << "Svart, black, s, b: Satsa p� svart." << std::endl;
		std::wcout << "1 - 36: Satsa p� den siffran." << std::endl;

		// wager kommer enbart inneh�lla siffror. Om den �r noll s� har anv�ndaren antingen matat in 0 eller bokst�ver.
		if (std::getline(std::wcin, input)) {
			std::wstringstream iss(input);
			iss >> wager;
		}

		// Omvandla input till enbart gemener f�r l�ttare j�mf�relser
		std::transform(input.begin(), input.end(), input.begin(), std::tolower);

		// Tom str�ng, spelaren skrev inte n�got
		if (input.empty())
		{
			std::wcout << "Illegal inmatning. Det hj�lper att skriva n�gonting." << std::endl;
			continue;
		}

		// Om input och wager inte �r samma s� �r det bokst�ver som skrivits in
		if (input != std::to_wstring(wager))
		{
			// Lambdor f�r att ta bort potentiell skr�pdata fr�n input. Alfanum bryr sig enbart om engelska tecken, s� m�ste g�ra flera stycken f�r att rensa
			input.erase(std::remove_if(input.begin(), input.end(), [](const wchar_t& c) { return std::isspace(c); }), input.end());
			input.erase(std::remove_if(input.begin(), input.end(), [](const wchar_t& c) { return std::isdigit(c); }), input.end());
			input.erase(std::remove_if(input.begin(), input.end(), [](const wchar_t& c) { return std::iscntrl(c); }), input.end());
			input.erase(std::remove_if(input.begin(), input.end(), [](const wchar_t& c) { return std::ispunct(c); }), input.end());

			// Hitta ifall ordet som skrivits in �r i arrayen med accepterade ord
			auto res = std::find_if(AcceptableWordsForColors.begin(), AcceptableWordsForColors.end(), [input](auto& item) { return input == item; });

			// I s� fall acceptera input
			if (res != AcceptableWordsForColors.end())
			{
				player.typeOfBet = input;
				acceptableInput = true;
			}
			else
			{
				std::wcout << "Urs�kta, jag f�rstod inte riktigt. Kan ni skriva om det?" << std::endl;
			}
		}
		// Spelaren har skrivit in siffror
		else
		{
			if (wager > 0 && wager < 37)
			{
				acceptableInput = true;
				player.typeOfBet = wager;
			}
			else
			{
				if (wager < 0)
				{
					std::wcout << "H�r spelar vi med br�den som enbart inneh�ller naturliga tal. Vad god mata in igen." << std::endl;
				}
				else
				{
					std::wcout << "Det finns inte stora nog bord i den h�r stan f�r oss b�da. Eller bara f�r dig. Var god mata in ett mindre v�rde." << std::endl;
				}
			}
		}

	} while (!acceptableInput);

	// G�r om loopen en g�ng med hur mycket man vill satsa
	acceptableInput = false;

	do
	{
		std::wcout << "Hur mycket pengar vill du satsa? Du kan satsa 100, 300 eller 500 kr." << std::endl;

		// wager kommer enbart inneh�lla siffror. Om den �r noll s� har anv�ndaren antingen matat in 0 eller bokst�ver.
		if (std::getline(std::wcin, input)) {
			std::wstringstream iss(input);
			iss >> wager;
		}

		if (wager == 100 || wager == 300 || wager == 500)
		{
			acceptableInput = true;
			std::wcout << L"Spelar f�r " + std::to_wstring(wager) + L" kronor" << std::endl;
			player.currentBet = wager;
		}
		else if (wager < 100)
		{
			std::wcout << "V�ga lev lite. Var god satsa om, mer pengar den h�r g�ngen." << std::endl;
		}
		else if (wager > 500)
		{
			std::wcout << "F�r att f�rhindra fusk s� finns det en gr�ns f�r hur mycket man f�r satsa. Var god satsa om." << std::endl;
		}
		else
		{
			std::wcout << "V�ra maskiner �r gamla och kan enbart jobba med runda, fina tal. Var god satsa om." << std::endl;
		}
	} while (!acceptableInput);
}

// R�kna ut vinsten och skriv ut den
void Roulette::CalculateWinnings()
{
	int randomNumber;

	if (std::holds_alternative<int>(player.typeOfBet))
	{
		randomNumber = GetRandomNumber();

		std::wcout << "Hjulet rullade siffran " << randomNumber << std::endl;

		if (std::get<int>(player.typeOfBet) == randomNumber)
		{
			player.money += player.currentBet * 10;
			player.totalWinnings += player.currentBet * 10;
			std::wcout << "Du gissade r�tt, och vinner d�rmed " << player.currentBet * 10 << " kronor!" << std::endl;
			std::wcout << "Du har nu " << player.money << " kronor totalt!" << std::endl;
			std::wcout << "Du har vunnit totalt " << player.totalWinnings << " kronor." << std::endl;
		}
		else
		{
			player.money -= player.currentBet;
			std::wcout << "Du gissade fel och f�rlorar din insats. Du har nu " << player.money << " kronor kvar." << std::endl;
		}
	}
	else if (std::holds_alternative<std::wstring>(player.typeOfBet))
	{
		randomNumber = GetRandomNumber(true);

		std::wcout << "Hjulet rullade f�rgen " << (randomNumber ? "svart" : "r�d") << std::endl;

		if (std::get<std::wstring>(player.typeOfBet) == L"red" || std::get<std::wstring>(player.typeOfBet) == L"r�d" || std::get<std::wstring>(player.typeOfBet) == L"r")
		{
			if (randomNumber == 0)
			{
				std::wcout << "Du gissade r�tt, och vinner d�rmed " << player.currentBet * 2 << " kronor!" << std::endl;
				player.money += player.currentBet * 2;
				player.totalWinnings += player.currentBet * 2;
				std::wcout << "Du har nu " << player.money << " kronor totalt!" << std::endl;
			}
			else
			{
				player.money -= player.currentBet;
				std::wcout << "Du gissade fel och f�rlorar din insats. Du har nu " << player.money << " kronor kvar." << std::endl;
			}
		}
		if (std::get<std::wstring>(player.typeOfBet) == L"black" || std::get<std::wstring>(player.typeOfBet) == L"svart" || std::get<std::wstring>(player.typeOfBet) == L"b" || std::get<std::wstring>(player.typeOfBet) == L"s")
		{
			if (randomNumber == 1)
			{
				std::wcout << "Du gissade r�tt, och vinner d�rmed " << player.currentBet * 2 << " kronor!" << std::endl;
				player.money += player.currentBet * 2;
				player.totalWinnings += player.currentBet * 2;
				std::wcout << "Du har nu " << player.money << " kronor totalt!" << std::endl;
			}
			else
			{
				player.money -= player.currentBet;
				std::wcout << "Du gissade fel och f�rlorar din insats. Du har nu " << player.money << " kronor kvar." << std::endl;
			}
		}
	}
}

// Nollst�ll gamestate och testa om spelet �r slut
void Roulette::EndRound()
{
	player.typeOfBet = NULL;
	player.latestBet = player.currentBet;
	player.currentBet = 0;

	if (player.money <= 0)
	{
		Roulette::SetGameRunning(false);
	}

	if (Roulette::GetGameRunning())
	{
		// Ta in text, konvertera till gemener, ta bort skr�ptecken
		std::wcout << "Vill du spela en g�ng till?" << std::endl;
		std::wstring input;
		std::getline(std::wcin, input);
		input.erase(std::remove_if(input.begin(), input.end(), [](const wchar_t& c) { return !std::isalnum(c); }), input.end());

		auto res = std::find_if(AcceptableWordsForChoice.begin(), AcceptableWordsForChoice.end(), [input](auto& item) { return input == item; });

		if (res != AcceptableWordsForChoice.end())
		{
			if (input == L"nej" || input == L"no" || input == L"n")
			{
				Roulette::SetGameRunning(false);
			}
		}
		else
		{
			std::wcout << "Jag f�rstod inte helt, men jag tar det som att du vill spela en g�ng till." << std::endl;
		}
	}

	std::wcout << std::endl;
}

// Ger en slumpm�ssig siffra fr�n MT:n. Ifall returnColor anges, returnera 0 eller 1 ist�llet, d�r 0 representerar r�tt och 1 representerar svart.
int Roulette::GetRandomNumber(bool returnColor)
{
	if (!returnColor)
	{
		// Modulo f�r att f� siffrorna till r�tt storlek, +1 f�r att 0 inte ska vara med.
		return RNG() % 36 + 1;
	}
	else
	{
		return RNG() % 2;
	}
}


void Roulette::Intro()
{
	// Ord som kan accepteras n�r spelaren skriver in en f�rg
	Roulette::AcceptableWordsForColors = {
		L"red",
		L"r�d",
		L"r",
		L"black",
		L"svart",
		L"s",
		L"b"
	};

	// Ord som kan accepteras n�r spelaren skriver in ja/nej
	Roulette::AcceptableWordsForChoice = {
		L"yes",
		L"ja",
		L"y",
		L"j",
		L"no",
		L"nej",
		L"n"
	};

	Roulette::gameRunning = true;
	Roulette::RNG().seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
}
// Intro
void Roulette::PrintIntro()
{
	// S�tta r�tt l�ge f�r konsollen s� att den enkelt kan skriva ut och ta in svenska tecken utan att beh�va g�ra escapes
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);

	std::wcout << "V�lkommen till roulettespelet. Du b�rjar med 1 000 kr i din ficka, " << std::endl;
	std::wcout << "och kommer nu ha m�jligheten att spela roulette f�r att �ka dina pengar " << std::endl;
	std::wcout << "eller f�rlora allt." << std::endl << std::endl;
	std::wcout << "Du kan g�ra f�ljande satsningar:" << std::endl;
	std::wcout << "- En siffra mellan 1 och 36, inklusive. Varje siffra har en p� 36 att bli vald." << std::endl;
	std::wcout << "- En f�rg, r�d eller svart. Varje f�rg har en p� tv� att bli vald." << std::endl;
	std::wcout << "Att v�lja r�tt siffra ger tillbaka tio g�nger insatsen, r�tt f�rg tv� g�nger." << std::endl;
	std::wcout << "Lycka till." << std::endl << std::endl;

	player.money = 1000;
}
// Outro
void Roulette::PrintOutro()
{
	std::wcout << "Farv�l, vi synes �ter i hasardspelshallarna.";
}
// Standard get/setter-par
bool Roulette::GetGameRunning()
{
	return Roulette::gameRunning;
}

void Roulette::SetGameRunning(bool state)
{
	Roulette::gameRunning = state;
}
// Alla includes
#include <iostream>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <algorithm>
#include "roulette.h"

// Ta in och hantera data från spelaren
void Roulette::GetUserInput()
{
	// Variabler som behövs i funktionen
	std::wstring input;
	bool acceptableInput = false;
	std::variant<int, std::wstring> bet;
	int wager = 0;

	do {
		// Strängar måste prefixas med L för att vara breda så de kan sättas ihop korrekt
		std::wcout << L"Du har " + player.PrintMoney() + L" kronor kvar att spela med." << std::endl;
		std::wcout << "Följande vad är accepterade:" << std::endl;
		std::wcout << "Röd, red, r: Satsa på röd." << std::endl;
		std::wcout << "Svart, black, s, b: Satsa på svart." << std::endl;
		std::wcout << "1 - 36: Satsa på den siffran." << std::endl;

		// wager kommer enbart innehålla siffror. Om den är noll så har användaren antingen matat in 0 eller bokstäver.
		if (std::getline(std::wcin, input)) {
			std::wstringstream iss(input);
			iss >> wager;
		}

		// Omvandla input till enbart gemener för lättare jämförelser
		std::transform(input.begin(), input.end(), input.begin(), std::tolower);

		// Tom sträng, spelaren skrev inte något
		if (input.empty())
		{
			std::wcout << "Illegal inmatning. Det hjälper att skriva någonting." << std::endl;
			continue;
		}

		// Om input och wager inte är samma så är det bokstäver som skrivits in
		if (input != std::to_wstring(wager))
		{
			// Lambdor för att ta bort potentiell skräpdata från input. Alfanum bryr sig enbart om engelska tecken, så måste göra flera stycken för att rensa
			input.erase(std::remove_if(input.begin(), input.end(), [](const wchar_t& c) { return std::isspace(c); }), input.end());
			input.erase(std::remove_if(input.begin(), input.end(), [](const wchar_t& c) { return std::isdigit(c); }), input.end());
			input.erase(std::remove_if(input.begin(), input.end(), [](const wchar_t& c) { return std::iscntrl(c); }), input.end());
			input.erase(std::remove_if(input.begin(), input.end(), [](const wchar_t& c) { return std::ispunct(c); }), input.end());

			// Hitta ifall ordet som skrivits in är i arrayen med accepterade ord
			auto res = std::find_if(AcceptableWordsForColors.begin(), AcceptableWordsForColors.end(), [input](auto& item) { return input == item; });

			// I så fall acceptera input
			if (res != AcceptableWordsForColors.end())
			{
				player.typeOfBet = input;
				acceptableInput = true;
			}
			else
			{
				std::wcout << "Ursäkta, jag förstod inte riktigt. Kan ni skriva om det?" << std::endl;
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
					std::wcout << "Här spelar vi med bräden som enbart innehåller naturliga tal. Vad god mata in igen." << std::endl;
				}
				else
				{
					std::wcout << "Det finns inte stora nog bord i den här stan för oss båda. Eller bara för dig. Var god mata in ett mindre värde." << std::endl;
				}
			}
		}

	} while (!acceptableInput);

	// Gör om loopen en gång med hur mycket man vill satsa
	acceptableInput = false;

	do
	{
		std::wcout << "Hur mycket pengar vill du satsa? Du kan satsa 100, 300 eller 500 kr." << std::endl;

		// wager kommer enbart innehålla siffror. Om den är noll så har användaren antingen matat in 0 eller bokstäver.
		if (std::getline(std::wcin, input)) {
			std::wstringstream iss(input);
			iss >> wager;
		}

		if (wager == 100 || wager == 300 || wager == 500)
		{
			acceptableInput = true;
			std::wcout << L"Spelar för " + std::to_wstring(wager) + L" kronor" << std::endl;
			player.currentBet = wager;
		}
		else if (wager < 100)
		{
			std::wcout << "Våga lev lite. Var god satsa om, mer pengar den här gången." << std::endl;
		}
		else if (wager > 500)
		{
			std::wcout << "För att förhindra fusk så finns det en gräns för hur mycket man får satsa. Var god satsa om." << std::endl;
		}
		else
		{
			std::wcout << "Våra maskiner är gamla och kan enbart jobba med runda, fina tal. Var god satsa om." << std::endl;
		}
	} while (!acceptableInput);
}

// Räkna ut vinsten och skriv ut den
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
			std::wcout << "Du gissade rätt, och vinner därmed " << player.currentBet * 10 << " kronor!" << std::endl;
			std::wcout << "Du har nu " << player.money << " kronor totalt!" << std::endl;
			std::wcout << "Du har vunnit totalt " << player.totalWinnings << " kronor." << std::endl;
		}
		else
		{
			player.money -= player.currentBet;
			std::wcout << "Du gissade fel och förlorar din insats. Du har nu " << player.money << " kronor kvar." << std::endl;
		}
	}
	else if (std::holds_alternative<std::wstring>(player.typeOfBet))
	{
		randomNumber = GetRandomNumber(true);

		std::wcout << "Hjulet rullade färgen " << (randomNumber ? "svart" : "röd") << std::endl;

		if (std::get<std::wstring>(player.typeOfBet) == L"red" || std::get<std::wstring>(player.typeOfBet) == L"röd" || std::get<std::wstring>(player.typeOfBet) == L"r")
		{
			if (randomNumber == 0)
			{
				std::wcout << "Du gissade rätt, och vinner därmed " << player.currentBet * 2 << " kronor!" << std::endl;
				player.money += player.currentBet * 2;
				player.totalWinnings += player.currentBet * 2;
				std::wcout << "Du har nu " << player.money << " kronor totalt!" << std::endl;
			}
			else
			{
				player.money -= player.currentBet;
				std::wcout << "Du gissade fel och förlorar din insats. Du har nu " << player.money << " kronor kvar." << std::endl;
			}
		}
		if (std::get<std::wstring>(player.typeOfBet) == L"black" || std::get<std::wstring>(player.typeOfBet) == L"svart" || std::get<std::wstring>(player.typeOfBet) == L"b" || std::get<std::wstring>(player.typeOfBet) == L"s")
		{
			if (randomNumber == 1)
			{
				std::wcout << "Du gissade rätt, och vinner därmed " << player.currentBet * 2 << " kronor!" << std::endl;
				player.money += player.currentBet * 2;
				player.totalWinnings += player.currentBet * 2;
				std::wcout << "Du har nu " << player.money << " kronor totalt!" << std::endl;
			}
			else
			{
				player.money -= player.currentBet;
				std::wcout << "Du gissade fel och förlorar din insats. Du har nu " << player.money << " kronor kvar." << std::endl;
			}
		}
	}
}

// Nollställ gamestate och testa om spelet är slut
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
		// Ta in text, konvertera till gemener, ta bort skräptecken
		std::wcout << "Vill du spela en gång till?" << std::endl;
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
			std::wcout << "Jag förstod inte helt, men jag tar det som att du vill spela en gång till." << std::endl;
		}
	}

	std::wcout << std::endl;
}

// Ger en slumpmässig siffra från MT:n. Ifall returnColor anges, returnera 0 eller 1 istället, där 0 representerar rött och 1 representerar svart.
int Roulette::GetRandomNumber(bool returnColor)
{
	if (!returnColor)
	{
		// Modulo för att få siffrorna till rätt storlek, +1 för att 0 inte ska vara med.
		return RNG() % 36 + 1;
	}
	else
	{
		return RNG() % 2;
	}
}


void Roulette::Intro()
{
	// Ord som kan accepteras när spelaren skriver in en färg
	Roulette::AcceptableWordsForColors = {
		L"red",
		L"röd",
		L"r",
		L"black",
		L"svart",
		L"s",
		L"b"
	};

	// Ord som kan accepteras när spelaren skriver in ja/nej
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
	// Sätta rätt läge för konsollen så att den enkelt kan skriva ut och ta in svenska tecken utan att behöva göra escapes
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);

	std::wcout << "Välkommen till roulettespelet. Du börjar med 1 000 kr i din ficka, " << std::endl;
	std::wcout << "och kommer nu ha möjligheten att spela roulette för att öka dina pengar " << std::endl;
	std::wcout << "eller förlora allt." << std::endl << std::endl;
	std::wcout << "Du kan göra följande satsningar:" << std::endl;
	std::wcout << "- En siffra mellan 1 och 36, inklusive. Varje siffra har en på 36 att bli vald." << std::endl;
	std::wcout << "- En färg, röd eller svart. Varje färg har en på två att bli vald." << std::endl;
	std::wcout << "Att välja rätt siffra ger tillbaka tio gånger insatsen, rätt färg två gånger." << std::endl;
	std::wcout << "Lycka till." << std::endl << std::endl;

	player.money = 1000;
}
// Outro
void Roulette::PrintOutro()
{
	std::wcout << "Farväl, vi synes åter i hasardspelshallarna.";
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
#pragma once
#include <variant>
#include <array>

// Skapa en Mersenne twister engine att använda som slumpgenerator för siffror, mata med nanosekunden sedan epoch som seed
std::mt19937 RNG(std::chrono::high_resolution_clock::now().time_since_epoch().count());

// Deklaration av alla funktioner
int GetRandomNumber(bool returnColor = 0);
void GetUserInput();
void CalculateWinnings();
void EndRound();
void PrintIntro();
void PrintOutro();

// Strukten som innehåller all data
struct PlayerData {
	int money = 0;
	int totalWinnings = 0;
	int currentBet = 0;
	int latestBet = 0;
	std::variant<int, std::wstring> typeOfBet;

	std::wstring PrintMoney()
	{
		return std::to_wstring(money);
	}
};

// Ord som kan accepteras när spelaren skriver in en färg
std::array<std::wstring, 10> AcceptableWordsForColors = {
	L"red",
	L"röd",
	L"r",
	L"black",
	L"svart",
	L"s",
	L"b"
};

// Ord som kan accepteras när spelaren skriver in ja/nej
std::array<std::wstring, 7> AcceptableWordsForChoice = {
	L"yes",
	L"ja",
	L"y",
	L"j",
	L"no",
	L"nej",
	L"n"
};

// Variabler som håller igång spelet och kommer ihåg information åt oss
bool gameRunning = true;
PlayerData* player = new PlayerData();
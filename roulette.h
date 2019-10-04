#pragma once

#include <variant>
#include <array>
#include <random>
#include <chrono>
#include <stdio.h>
#include <stdio.h>
#include <string>

namespace Roulette
{
	// Deklaration av alla funktioner
	int GetRandomNumber(bool returnColor = 0);
	void GetUserInput();
	void CalculateWinnings();
	void EndRound();
	void Intro();
	void Outro();
	void PrintIntro();
	void PrintOutro();
	bool GetGameRunning();
	void SetGameRunning(bool state);

	// Deklaration av alla strukturer
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

	extern std::array<std::wstring, 10> AcceptableWordsForColors;
	extern std::array<std::wstring, 7> AcceptableWordsForChoice;

	//Deklaration av alla variabler
	PlayerData player;
	extern bool gameRunning;
	extern std::mt19937 RNG();

}
#include "roulette.h"

// Entrypoint
int main()
{
	// Spelet börjar
	Roulette::Intro();

	// Spelloop
	while (Roulette::GetGameRunning())
	{
		// Varje omgång ska vi: Ta in data, räkna ut vinsten, och sedan se över våran status
		Roulette::GetUserInput();
		Roulette::CalculateWinnings();
		Roulette::EndRound();
	}

	// Spelet är avslutat, antingen med vilja eller genom att förlora alla pengar
	Roulette::Outro();

	return 0;
}
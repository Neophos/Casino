#include "roulette.h"

// Entrypoint
int main()
{
	// Spelet b�rjar
	Roulette::Intro();

	// Spelloop
	while (Roulette::GetGameRunning())
	{
		// Varje omg�ng ska vi: Ta in data, r�kna ut vinsten, och sedan se �ver v�ran status
		Roulette::GetUserInput();
		Roulette::CalculateWinnings();
		Roulette::EndRound();
	}

	// Spelet �r avslutat, antingen med vilja eller genom att f�rlora alla pengar
	Roulette::Outro();

	return 0;
}
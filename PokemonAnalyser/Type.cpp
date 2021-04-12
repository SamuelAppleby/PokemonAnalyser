#include "Type.h"
#include <algorithm>
#include <cctype>
#include <iostream>

Type::Type(PokemonType t)
{
	type = t;
	for (PokemonType s = (PokemonType)0; s != NUMTYPES; s = (PokemonType)(s + 1)) {
		offensive[s] = NO_EFFECT;
		resistance[s] = NO_EFFECT;
	}
	offensiveStat = 0;
	defensiveStat = 0;
}

void Type::Analyse()
{
	for (auto& o : offensive) {
		offensiveOccurence[o.second]++;
	}

	for (auto const& x : offensiveOccurence)
	{
		switch (x.first) {
		case NO_EFFECT: offensiveStat -= (x.second * 2); break;
		case NOT_VERY_EFFECTIVE: offensiveStat -= x.second; break;
		case EFFECTIVE: break;
		case SUPER_EFFECTIVE: offensiveStat += x.second; break;
		}
	}

	for (auto& o : resistance) {
		resistanceOccurence[o.second]++;
	}

	for (auto const& x : resistanceOccurence)
	{
		switch (x.first) {
		case NO_EFFECT: defensiveStat += (x.second * 2); break;
		case NOT_VERY_EFFECTIVE: defensiveStat += x.second; break;
		case EFFECTIVE: break;
		case SUPER_EFFECTIVE: defensiveStat -= x.second; break;
		}
	}

	std::cout << "Offense: " << offensiveStat << std::endl;
	std::cout << "Defence: " << defensiveStat << std::endl;
}



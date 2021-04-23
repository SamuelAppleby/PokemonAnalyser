/*		 Created By Samuel Buzz Appleby
 *               12/04/2021
 *			  TypeImplementation			    */
#include "MonoType.h"

MonoType::MonoType(PokemonType t)
{
	type = t;
	offensiveStat = 0;
	defensiveStat = 0;
	totalStat = 0;
}

void MonoType::Analyse()
{
	for (int i = 0; i < 2; ++i) {
		float& currentStat = !i ? offensiveStat : defensiveStat;
		std::map<PokemonType, DamageResult>& map = !i ? offensive : resistance;
		std::map<DamageResult, int>& damageMap = !i ? offensiveOccurence : resistanceOccurence;
		for (auto const& x : map)
		{
			switch (x.second) {
			case ZERO:
				break;
			case HALF:
				currentStat += 0.5;
				break;
			case ONE:
				currentStat += 1;
				break;
			case DOUBLE:
				currentStat += 2;
				break;
			}
			damageMap[x.second]++;
		}
		currentStat /= map.size();
	}
	totalStat = (offensiveStat + (1 / defensiveStat)) / 2;
}

std::ostream& operator<<(std::ostream& strm, const MonoType& a) {
	switch (a.GetType()) {
	case NORMAL: strm << "Normal"; break;
	case FIRE: strm << "Fire"; break;
	case WATER: strm << "Water"; break;
	case ELECTRIC: strm << "Electric"; break;
	case GRASS: strm << "Grass"; break;
	case ICE: strm << "Ice"; break;
	case FIGHTING: strm << "Fighting"; break;
	case POISON: strm << "Poison"; break;
	case GROUND: strm << "Ground"; break;
	case PSYCHIC: strm << "Psychic"; break;
	case FLYING: strm << "Flying"; break;
	case BUG: strm << "Bug"; break;
	case ROCK: strm << "Rock"; break;
	case GHOST: strm << "Ghost"; break;
	case DRAGON: strm << "Dragon"; break;
	case DARK: strm << "Dark"; break;
	case STEEL: strm << "Steel"; break;
	case FAIRY: strm << "Fairy"; break;
	}
	return strm;
}

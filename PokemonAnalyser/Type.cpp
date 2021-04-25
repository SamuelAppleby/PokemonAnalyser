#include "Type.h"

Type::Type(std::pair<MonoType, MonoType> t)
{
	typePair = t;
	offensiveStat = 0;
	defensiveStat = 0;
	totalStat = 0;
}

void Type::Analyse()
{
	for (int i = 0; i < 2; ++i) {
		float& currentStat = !i ? offensiveStat : defensiveStat;
		std::map<std::pair<PokemonType, PokemonType>, DamageResult>& map = !i ? offensive : resistance;
		std::map<DamageResult, int>& damageMap = !i ? offensiveOccurence : resistanceOccurence;
		for (auto const& x : map)
		{
			switch (x.second) {
			case ZERO:
				break;
			case QUARTER:
				currentStat += 0.25;
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
			case QUADRUPLE:
				currentStat += 4;
				break;
			}
			damageMap[x.second]++;
		}
		currentStat /= map.size();
	}
	totalStat = (offensiveStat + (1 / defensiveStat)) / 2;
}

std::ostream& operator<<(std::ostream& strm, const Type& a) {
	for (int i = 0; i < 2; ++i) {
		PokemonType type = i ? a.GetTypePair().first.GetType() : a.GetTypePair().second.GetType();
		switch (type) {
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
		if(!i) 	strm << "/";
	}
	return strm;
}
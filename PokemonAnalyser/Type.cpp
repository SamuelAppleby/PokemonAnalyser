#include "Type.h"

Type::Type(std::pair<BaseType, BaseType> t)
{
	type = t;
	offensiveStat = 0;
	defensiveStat = 0;
	totalStat = 0;

	for (PokemonType s = (PokemonType)0; s != NUMTYPES; s = (PokemonType)(s + 1)) {
		DamageResult resist1 = type.first.resistance.find(s)->second;
		DamageResult resist2 = type.second.resistance.find(s)->second;
		if (resist1 == NO_EFFECT || resist2 == NO_EFFECT) {
			resistance[s] = 0;
			continue;
		}
		std::pair<DamageResult, DamageResult> resistPair(resist1, resist2);
		switch (resistPair.first) {
		case NOT_VERY_EFFECTIVE:
			switch (resistPair.second) {
			case NOT_VERY_EFFECTIVE:
				resistance[s] = 0.25;
				break;
			case EFFECTIVE:
				resistance[s] = 0.5;
				break;
			case SUPER_EFFECTIVE:
				resistance[s] = 1;
				break;
			}
			break;
		case EFFECTIVE:
			switch (resistPair.second) {
			case NOT_VERY_EFFECTIVE:
				resistance[s] = 0.5;
				break;
			case EFFECTIVE:
				resistance[s] = 1;
				break;
			case SUPER_EFFECTIVE:
				resistance[s] = 2;
				break;
			}
			break;
		case SUPER_EFFECTIVE:
			switch (resistPair.second) {
			case NOT_VERY_EFFECTIVE:
				resistance[s] = 1;
				break;
			case EFFECTIVE:
				resistance[s] = 2;
				break;
			case SUPER_EFFECTIVE:
				resistance[s] = 4;
				break;
			}
			break;
		}
	}
}

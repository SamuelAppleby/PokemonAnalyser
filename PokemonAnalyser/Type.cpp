/*		 Created By Samuel Buzz Appleby
 *               12/04/2021
 *			  TypeImplementation			    */
#include "Type.h"

Type::Type(PokemonType t)
{
	type = t;
	for (PokemonType s = (PokemonType)0; s != NUMTYPES; s = (PokemonType)(s + 1)) {
		offensive[s] = NO_EFFECT;
		resistance[s] = NO_EFFECT;
	}
	for (DamageResult s = (DamageResult)0; s != NUMDAMAGES; s = (DamageResult)(s + 1)) {
		offensiveOccurence[s] = 0;
		resistanceOccurence[s] = 0;
	}
	offensiveStat = 0;
	defensiveStat = 0;
	totalStat = 0;
}

void Type::Analyse()
{
	for (int i = 0; i < 2; ++i) {
		std::map<PokemonType, DamageResult>& map = !i ? offensive : resistance;
		std::map<DamageResult, int>& damageMap = !i ? offensiveOccurence : resistanceOccurence;
		for (auto const& x : map)
		{
			float& currentStat = !i ? offensiveStat : defensiveStat;
			switch (x.second) {
			case NO_EFFECT:
				currentStat = !i ? currentStat - 2 : currentStat + 2;
				break;
			case NOT_VERY_EFFECTIVE:
				currentStat = !i ? currentStat - 1 : currentStat + 1;
				break;
			case EFFECTIVE:
				break;
			case SUPER_EFFECTIVE:
				currentStat = !i ? currentStat + 1 : currentStat - 1;
				break;
			}
			damageMap[x.second]++;
		}
	}
	totalStat = defensiveStat + offensiveStat;
}



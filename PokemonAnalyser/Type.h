#pragma once
#include <vector>
#include <map>
#include <string>

enum DamageResult { NO_EFFECT, NOT_VERY_EFFECTIVE, EFFECTIVE, SUPER_EFFECTIVE };
enum PokemonType { NORMAL, FIRE, WATER, ELECTRIC, GRASS, ICE, FIGHTING, POISON, GROUND, FLYING, PSYCHIC, BUG, ROCK, GHOST, DRAGON, DARK, STEEL, FAIRY, NUMTYPES };

class Type
{
public:
	Type(){}
	Type(PokemonType t);
	void Analyse();
	PokemonType GetType() const {
		return type;
	}
	std::map<PokemonType, DamageResult>& GetOffensives() {
		return offensive;
	}
	std::map<PokemonType, DamageResult>& GetDefensives() {
		return resistance;
	}

private:
	PokemonType type;

	std::map<PokemonType, DamageResult> offensive;
	std::map<DamageResult, int> offensiveOccurence;
	float offensiveStat;

	std::map<PokemonType, DamageResult> resistance;
	std::map<DamageResult, int> resistanceOccurence;
	float defensiveStat;
};


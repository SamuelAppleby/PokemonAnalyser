#pragma once
#include <vector>
#include <map>
#include <string>

enum DamageResult { NO_EFFECT, NOT_VERY_EFFECTIVE, EFFECTIVE, SUPER_EFFECTIVE, NUMDAMAGES };
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

	std::map<DamageResult, int>& GetOffensiveOccurences() {
		return offensiveOccurence;
	}

	float GetOffensiveStat() const {
		return offensiveStat;
	}

	std::map<PokemonType, DamageResult>& GetDefensives() {
		return resistance;
	}

	std::map<DamageResult, int>& GetResistanceOccurences() {
		return resistanceOccurence;
	}

	float GetDefensiveStat() const {
		return defensiveStat;
	}

	float GetTotalStat() const {
		return totalStat;
	}

	bool operator< (const Type& other) const {
		return totalStat > other.totalStat;
	}

private:
	PokemonType type;

	std::map<PokemonType, DamageResult> offensive;
	std::map<DamageResult, int> offensiveOccurence;
	float offensiveStat;

	std::map<PokemonType, DamageResult> resistance;
	std::map<DamageResult, int> resistanceOccurence;
	float defensiveStat;

	int totalStat;
};


/*		 Created By Samuel Buzz Appleby
 *               12/04/2021
 *			    TypeDefinition			    */
#pragma once
#include <vector>
#include <map>
#include <string>
#include <iostream>
enum DamageResult { ZERO, QUARTER, HALF, ONE, DOUBLE, QUADRUPLE, NUMDAMAGES };
enum PokemonType { NORMAL, FIRE, WATER, ELECTRIC, GRASS, ICE, FIGHTING, POISON, GROUND, FLYING, PSYCHIC, BUG, ROCK, GHOST, DRAGON, DARK, STEEL, FAIRY, NUMTYPES };

class BaseType
{
public:
	BaseType() {}

	std::map<DamageResult, int> GetOffensiveOccurences() {
		return offensiveOccurence;
	}

	float GetOffensiveStat() const {
		return offensiveStat;
	}

	std::map<DamageResult, int> GetResistanceOccurences() {
		return resistanceOccurence;
	}

	float GetDefensiveStat() const {
		return defensiveStat;
	}

	float GetTotalStat() const {
		return totalStat;
	}

	bool operator< (const BaseType& other) const {
		return totalStat > other.totalStat;
	}

protected:
	std::map<DamageResult, int> offensiveOccurence;
	float offensiveStat;

	std::map<DamageResult, int> resistanceOccurence;
	float defensiveStat;

	float totalStat;
};


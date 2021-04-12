#pragma once
#include <fstream>
#include <iostream>  
#include <algorithm>
#include <set>
#include <vector>
#include "Type.h"

class TypeManager
{
public:
	TypeManager(std::ifstream& stream);
	Type* FindTypeInList(PokemonType type);
	PokemonType ConvertTypeName(std::string name);
	DamageResult ConvertDamageName(std::string damage);
	std::string TypeToString(PokemonType type);
	std::string DamageToString(DamageResult type);
	void AnalyseTypes();
	void Summary();
	void OutputResults();
	void SortTypesByOffense(std::vector<Type>& sort);
	void SortTypesByDefense(std::vector<Type>& sort);
	void SortTypesByTotal(std::vector<Type>& sort);

private:
	std::vector<Type> types;
	std::vector<Type> typesByOffence;
	std::vector<Type> typesByDefence;
	std::vector<Type> typesByTotal;
};


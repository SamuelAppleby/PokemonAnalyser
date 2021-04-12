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
	void AnalyseResults();
	void OutputResults();
private:
	std::vector<Type> types;
};


/*		 Created By Samuel Buzz Appleby
 *               12/04/2021
 *			TypeManagerDefinition			    */
#pragma once
#include <fstream>
#include <iostream>  
#include <algorithm>
#include <vector>
#include <string>
#include "BaseType.h"
#include "Type.h"

class TypeManager
{
public:
	TypeManager(std::ifstream& stream);
	BaseType* FindTypeInList(PokemonType type);
	PokemonType ConvertTypeName(std::string name);
	DamageResult ConvertDamageName(std::string damage);
	std::string TypeToString(PokemonType type);
	std::string DamageToString(DamageResult type);
	void AnalyseTypes();
	void Summary();
	void OutputResults();
	void CreateDualTypes();
	void SortTypesByOffense(std::vector<BaseType>& sort);
	void SortTypesByDefense(std::vector<BaseType>& sort);
	void SortTypesByTotal(std::vector<BaseType>& sort);

private:
	std::vector<BaseType> types;
	std::vector<BaseType> typesByOffence;
	std::vector<BaseType> typesByDefence;
	std::vector<BaseType> typesByTotal;

	std::vector<Type> dualTypes;
};


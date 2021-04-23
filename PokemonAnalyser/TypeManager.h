/*		 Created By Samuel Buzz Appleby
 *               12/04/2021
 *			TypeManagerDefinition			    */
#pragma once
#include <fstream>
#include <iostream>  
#include <algorithm>
#include <vector>
#include <string>
#include "MonoType.h"
#include "Type.h"

class TypeManager
{
public:
	TypeManager(std::ifstream& stream);
	MonoType* FindTypeInList(PokemonType type);
	PokemonType ConvertTypeName(std::string name);
	DamageResult ConvertDamageName(std::string damage);
	std::string TypeToString(PokemonType type);
	std::string TypeToString(std::pair<MonoType, MonoType> types);
	std::string DamageToString(DamageResult type);
	void AnalyseTypes();
	void Summary(bool dualType = false);
	void OutputResults(bool dualType = false);
	void AnalyseDualTypes();
	void SortTypesByOffense(std::vector<MonoType>& sort);
	void SortTypesByOffense(std::vector<Type>& sort);
	void SortTypesByDefense(std::vector<MonoType>& sort);
	void SortTypesByDefense(std::vector<Type>& sort);
	void SortTypesByTotal(std::vector<MonoType>& sort);
	void SortTypesByTotal(std::vector<Type>& sort);

private:
	std::vector<MonoType> types;
	std::vector<MonoType> baseTypesByOffence;
	std::vector<MonoType> baseTypesByDefence;
	std::vector<MonoType> baseTypesByTotal;

	std::vector<Type> dualTypes;
	std::vector<Type> dualTypesByOffence;
	std::vector<Type> dualTypesByDefence;
	std::vector<Type> dualTypesByTotal;
};


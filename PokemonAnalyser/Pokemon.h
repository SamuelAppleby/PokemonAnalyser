#pragma once
#include "Type.h"
class Pokemon
{
public:
	Pokemon(std::string pokeNum, std::string n, Type t);
	std::string GetPokedexNumber() const {
		return pokedexNum;
	}
	std::string GetName() const {
		return name;
	}
	Type GetType() const {
		return type;
	}
private:
	std::string pokedexNum;
	std::string name;
	Type type;
};


#pragma once
#include "Pokemon.h"
#include <fstream>
class PokemonManager
{
public:
	PokemonManager(std::ifstream& stream);
	void PrintPokemon();
private:
	std::vector<Pokemon> pokemonList;
};


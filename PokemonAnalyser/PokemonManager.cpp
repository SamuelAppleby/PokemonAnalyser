#include "PokemonManager.h"
#include "TypeManager.h"
#include <sstream>
#include <iomanip>

PokemonManager::PokemonManager(std::ifstream& stream)
{
	std::string line;
	int pokedexNum = 1;
	while (getline(stream, line)) {
		line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
		int tokenNo = 0;
		size_t position = 0;
		std::string pokedexNum;
		std::string name;
		Type* pokeType = nullptr;
		std::pair<PokemonType, PokemonType> t;
		char delimiter = '|';
		std::stringstream ss(line);
		std::string str;
		while (getline(ss, str, delimiter)) {
			if (!tokenNo) pokedexNum = str;
			else if(tokenNo == 1) name = str;
			else {
				delimiter = '/';
				std::stringstream ss1(str);
				int typeNo = 0;
				while (getline(ss1, str, delimiter)) {
					if (!typeNo) t.first = TypeManager::ConvertTypeName(str);					
					else t.second = TypeManager::ConvertTypeName(str);
					typeNo++;
				}
				if (typeNo == 1) t.second = t.first;
				pokeType = TypeManager::FindTypeInList(t);
			}
			tokenNo++;
		}
		if(pokeType) pokemonList.push_back(Pokemon(pokedexNum, name, *pokeType));
	}
	++pokedexNum;
}

void PokemonManager::PrintPokemon()
{
	std::cout << std::setw(14) << "Pokedex Number" << std::setw(12) << "Pokemon" << std::setw(23) << "Type" << std::endl;

	for (const auto& p : pokemonList) {
		std::cout << std::setw(15) << p.GetPokedexNumber() << std::setw(12) << p.GetName() << std::setw(10)
			<< TypeManager::TypeToString(p.GetType().GetTypePair().first.GetType()) << std::setw(3) << "/" << std::setw(10)
			<< TypeManager::TypeToString(p.GetType().GetTypePair().second.GetType()) << std::endl;
	}
}

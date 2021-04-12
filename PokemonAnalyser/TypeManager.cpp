#include "TypeManager.h"
#include <string>

TypeManager::TypeManager(std::ifstream& stream) 
{
	std::string line;
	int lineNo = 0;
	std::string delimiter = "|";
	while (getline(stream, line)) {
		line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
		if (!lineNo) {
			size_t position = 0;
			std::string token;
			while ((position = line.find(delimiter)) != std::string::npos) {
				token = line.substr(0, position);
				Type t(ConvertTypeName(token));
				types.push_back(t);
				line.erase(0, position + delimiter.length());
			}
		}	
		else {
			int tokenNo = 0;
			size_t position = 0;
			std::string token;
			Type* currentType = nullptr;
			while ((position = line.find(delimiter)) != std::string::npos) {
				token = line.substr(0, position);
				if (!tokenNo) currentType = FindTypeInList(ConvertTypeName(token));
				else {
					PokemonType other = PokemonType(tokenNo - 1);
					Type* otherType = FindTypeInList(other);
					DamageResult dmg = ConvertDamageName(token);
					currentType->GetOffensives()[other] = dmg;
					otherType->GetDefensives()[currentType->GetType()] = dmg;
				}
				line.erase(0, position + delimiter.length());
				tokenNo++;
			}
		}
		++lineNo;
	}
}

Type* TypeManager::FindTypeInList(PokemonType type)
{
	for (auto& t : types) {
		if (type == t.GetType()) return &t;
	}
	return nullptr;
}

PokemonType TypeManager::ConvertTypeName(std::string name)
{
	std::transform(name.begin(), name.end(), name.begin(),
		[](unsigned char c) { return std::tolower(c); });
	if (name == "normal") return PokemonType::NORMAL;
	if (name == "fire") return PokemonType::FIRE;
	if (name == "water") return PokemonType::WATER;
	if (name == "electric") return PokemonType::ELECTRIC;
	if (name == "grass") return PokemonType::GRASS;
	if (name == "ice") return PokemonType::ICE;
	if (name == "fighting") return PokemonType::FIGHTING;
	if (name == "poison") return PokemonType::POISON;
	if (name == "ground") return PokemonType::GROUND;
	if (name == "psychic") return PokemonType::PSYCHIC;
	if (name == "flying") return PokemonType::FLYING;
	if (name == "bug") return PokemonType::BUG;
	if (name == "rock") return PokemonType::ROCK;
	if (name == "ghost") return PokemonType::GHOST;
	if (name == "dragon") return PokemonType::DRAGON;
	if (name == "dark") return PokemonType::DARK;
	if (name == "steel") return PokemonType::STEEL;
	if (name == "fairy") return PokemonType::FAIRY;
	return PokemonType();
}

DamageResult TypeManager::ConvertDamageName(std::string damage)
{
	std::transform(damage.begin(), damage.end(), damage.begin(),
		[](unsigned char c) { return std::tolower(c); });
	if (damage == "0") return DamageResult::NO_EFFECT;
	if (damage == "0.5") return DamageResult::NOT_VERY_EFFECTIVE;
	if (damage == "1") return DamageResult::EFFECTIVE;
	if (damage == "2") return DamageResult::SUPER_EFFECTIVE;
	return DamageResult();
}

std::string TypeManager::TypeToString(PokemonType type)
{
	switch (type) {
	case NORMAL: return "Normal"; break;
	case FIRE: return "Fire"; break;
	case WATER: return "Water"; break;
	case ELECTRIC: return "Electric"; break;
	case GRASS: return "Grass"; break;
	case ICE: return "Ice"; break;
	case FIGHTING: return "Fighting"; break;
	case POISON: return "Poison"; break;
	case GROUND: return "Ground"; break;
	case PSYCHIC: return "Psychic"; break;
	case FLYING: return "Flying"; break;
	case BUG: return "Bug"; break;
	case ROCK: return "Rock"; break;
	case GHOST: return "Ghost"; break;
	case DRAGON: return "Dragon"; break;
	case DARK: return "Dark"; break;
	case STEEL: return "Steel"; break;
	case FAIRY: return "Fairy"; break;
	}
	return std::string();
}

std::string TypeManager::DamageToString(DamageResult type)
{
	switch (type) {
	case NO_EFFECT: return "NO EFFECT"; break;
	case NOT_VERY_EFFECTIVE: return "NOT VERY EFFECTIVE"; break;
	case EFFECTIVE: return "EFFECTIVE"; break;
	case SUPER_EFFECTIVE: return "SUPER EFFECTIVE"; break;
	}
	return std::string();
}

void TypeManager::AnalyseResults()
{
	for (auto& t : types) {
		t.Analyse();
	}
}

void TypeManager::OutputResults()
{
	std::cout << "Results" << std::endl;
	for (auto& t : types) {
		std::cout << "\n\n\n--" << TypeToString(t.GetType()) << "--" << std::endl;
		std::cout << "|    Attacking    |" << std::endl;
		std::string noEffect("No Effect: ");
		std::string notEffective("Not Very Effective: ");
		std::string effective("Effective: ");
		std::string superEffective("Super Effective: ");
		for (auto const& x : t.GetOffensives())
		{
			switch (x.second) {
			case NO_EFFECT:
				noEffect.append(TypeToString(x.first) + " | ");
				break;
			case NOT_VERY_EFFECTIVE:
				notEffective.append(TypeToString(x.first) + " | ");
				break;
			case EFFECTIVE:
				effective.append(TypeToString(x.first) + " | ");
				break;
			case SUPER_EFFECTIVE:
				superEffective.append(TypeToString(x.first) + " | ");
				break;
			}
		}
		std::cout << noEffect << std::endl << notEffective << std::endl << effective << std::endl << superEffective;

		std::cout << "\n\n|    Defending    |" << std::endl;
		noEffect = "No Effect: ";
		notEffective = "Not Very Effective: ";
		effective = "Effective: ";
		superEffective = "Super Effective: ";
		for (auto const& x : t.GetDefensives())
		{
			switch (x.second) {
			case NO_EFFECT:
				noEffect.append(TypeToString(x.first) + " | ");
				break;
			case NOT_VERY_EFFECTIVE:
				notEffective.append(TypeToString(x.first) + " | ");
				break;
			case EFFECTIVE:
				effective.append(TypeToString(x.first) + " | ");
				break;
			case SUPER_EFFECTIVE:
				superEffective.append(TypeToString(x.first) + " | ");
				break;
			}
		}
		std::cout << noEffect << std::endl << notEffective << std::endl << effective << std::endl << superEffective;
	}
}

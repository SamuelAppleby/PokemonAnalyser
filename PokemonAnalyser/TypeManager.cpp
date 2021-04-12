#include "TypeManager.h"
#include <string>
#include <iomanip>
#include <fstream>
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

void TypeManager::AnalyseTypes()
{
	for (auto& t : types) {
		t.Analyse();
	}
	typesByOffence = types;
	SortTypesByOffense(typesByOffence);

	typesByDefence = types;
	SortTypesByDefense(typesByDefence);

	typesByTotal = types;
	SortTypesByTotal(typesByTotal);
}

void TypeManager::Summary()
{
	for (int i = 0; i < 3; ++i) {
		std::string title = !i ? "SORTED BY OFFENSIVE STAT" : i == 1 ? "SORTED BY DEFENSIVE STAT" : "SORTED BY TOTAL STAT";
		title.insert(title.begin(), 24 - title.length(), ' ');

		std::cout << std::endl <<  title << std::endl;
		std::vector<Type> sorted = !i ? typesByOffence : i == 1 ? typesByDefence : typesByTotal;

		for (auto& t : sorted) {
			int value = !i ? (int)t.GetOffensiveStat() : i == 1 ? (int)t.GetDefensiveStat() :(int)t.GetTotalStat();
			std::string info = TypeToString(t.GetType()) + ":" + std::to_string(value);
			info.insert(info.begin(), title.length() - info.length(), ' ');
			std::cout << info << std::endl;
		}
	}

	std::ofstream outFile;
	outFile.open("summary.csv");
	if (!outFile) {
		std::cout << "Cannot Create File";
		return;
	}
	for (int i = 0; i < 3; ++i) {
		std::string title = !i ? "SORTED BY OFFENSIVE STAT" : i == 1 ? "SORTED BY DEFENSIVE STAT" : "SORTED BY TOTAL STAT";
		outFile << title << ",";
	}
	outFile << "\n";
	for (int i = 0; i < types.size(); ++i) {
		for (int j = 0; j < 3; ++j) {
			Type current = !j ? typesByOffence.at(i) : j == 1 ? typesByDefence.at(i) : typesByTotal.at(i);
			int value = !j ? current.GetOffensiveStat() : j == 1 ? current.GetDefensiveStat() : current.GetTotalStat();
			outFile << TypeToString(current.GetType()) + ":" + std::to_string(value) << ",";
		}
		outFile << "\n";
	}
	outFile.close();
}

void TypeManager::OutputResults()
{
	std::ofstream outFile;
	outFile.open("breakdown.csv");
	if (!outFile) {
		std::cout << "Cannot Create File";
		return;
	}

	for (auto& t : types) {
		std::cout << "\n------------------------" << TypeToString(t.GetType()) << "------------------------" << std::endl;
		outFile << "------------------------" << TypeToString(t.GetType()) << "------------------------" << std::endl;;
		for (int i = 0; i < 2; ++i) {
			std::vector<std::string> effects;
			for (DamageResult s = (DamageResult)0; s != NUMDAMAGES; s = (DamageResult)(s + 1))
			{
				effects.push_back(std::string());
			}

			std::map<PokemonType, DamageResult> map = !i ? t.GetOffensives() : t.GetDefensives();
			for (auto const& x : map)
			{
				std::string value = TypeToString(x.first);
				switch (x.second) {
				case NO_EFFECT:
					effects.at(0).append(" , " + value);
					break;
				case NOT_VERY_EFFECTIVE:
					effects.at(1).append(" , " + value);
					break;
				case EFFECTIVE:
					effects.at(2).append(" , " + value);
					break;
				case SUPER_EFFECTIVE:
					effects.at(3).append(" , " + value);
					break;
				}
			}

			if (!i) {
				std::cout << "|      Attacking      |" << std::endl;
				outFile << "|      Attacking      |\n";
			}
			else {
				std::cout << "\n|      Defending      |\n" << std::endl;
				outFile << "\n|      Defending      |\n";
			}
			std::map<DamageResult, int> damageMap = !i ? t.GetOffensiveOccurences() : t.GetResistanceOccurences();
			for (DamageResult s = (DamageResult)0; s != NUMDAMAGES; s = (DamageResult)(s + 1))
			{
				if (damageMap[s]) {
					std::string info = DamageToString(s) + "[" + std::to_string(damageMap[s]) + "]: ";
					std::cout << info;
					outFile << info;
					std::cout << effects.at(s) << std::endl;
					outFile << effects.at(s) << "\n";
				}
			}
			float stat = !i ? t.GetOffensiveStat() : t.GetDefensiveStat();
			std::cout << "Total Score: " << stat << std::endl;
			outFile << "Total Score: " << stat << "\n";
		}
		outFile << "\n";
	}
	outFile.close();
}

void TypeManager::SortTypesByOffense(std::vector<Type>& sort)
{
	std::sort(sort.begin(), sort.end(), [](const Type& lhs, const Type& rhs) {
		return lhs.GetOffensiveStat() > rhs.GetOffensiveStat();
	});
}

void TypeManager::SortTypesByDefense(std::vector<Type>& sort)
{
	std::sort(sort.begin(), sort.end(), [](const Type& lhs, const Type& rhs) {
		return lhs.GetDefensiveStat() > rhs.GetDefensiveStat();
	});
}

void TypeManager::SortTypesByTotal(std::vector<Type>& sort)
{
	std::sort(sort.begin(), sort.end());
}





/*		 Created By Samuel Buzz Appleby
 *               12/04/2021
 *		  TypeManagerImplementation			    */
#include "TypeManager.h"
#include <iomanip>
#include <sstream>
std::vector<Type> TypeManager::dualTypes;
TypeManager::TypeManager(std::ifstream& stream) 
{
	std::string line;
	int lineNo = 0;
	char delimiter = '|';
	std::string str;
	while (getline(stream, line)) {
		line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
		if (!lineNo) {
			size_t position = 0;
			std::stringstream ss(line);
			while (getline(ss, str, delimiter)) {
				MonoType t(ConvertTypeName(str));
				types.push_back(t);
			}
		}	
		else {
			int tokenNo = 0;
			MonoType* currentType = nullptr;
			std::stringstream ss(line);
			while (getline(ss, str, delimiter)) {
				if (!tokenNo) currentType = FindTypeInList(ConvertTypeName(str));
				else {
					PokemonType other = PokemonType(tokenNo - 1);
					MonoType* otherType = FindTypeInList(other);
					DamageResult dmg = ConvertDamageName(str);
					currentType->offensive[other] = dmg;
					otherType->resistance[currentType->GetType()] = dmg;
				}
				tokenNo++;
			}
		}
		++lineNo;
	}
	for (int i = 0; i < types.size(); i++) {
		for (int j = i; j < types.size(); j++) {
			Type t(std::pair<MonoType, MonoType>(types.at(i), types.at(j)));
			dualTypes.push_back(t);
		}
	}
}

MonoType* TypeManager::FindTypeInList(PokemonType type)
{
	for (auto& t : types) {
		if (type == t.GetType()) return &t;
	}
	return nullptr;
}

Type* TypeManager::FindTypeInList(std::pair<PokemonType, PokemonType> typePair)
{
	for (auto& t : dualTypes) {
		if (t.GetTypePair().first.GetType() == typePair.first && t.GetTypePair().second.GetType() == typePair.second
			|| t.GetTypePair().first.GetType() == typePair.second && t.GetTypePair().second.GetType() == typePair.first) return &t;
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
	if (damage == "0") return DamageResult::ZERO;
	if (damage == "0.5") return DamageResult::HALF;
	if (damage == "1") return DamageResult::ONE;
	if (damage == "2") return DamageResult::DOUBLE;
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
}

std::string TypeManager::TypeToString(std::pair<MonoType, MonoType> types)
{
	std::string str;
	for (int i = 0; i < 2; ++i) {
		PokemonType type = i ? types.first.GetType() : types.second.GetType();
		switch (type) {
		case NORMAL: str.append("Normal"); break;
		case FIRE: str.append("Fire"); break;
		case WATER: str.append("Water"); break;
		case ELECTRIC: str.append("Electric"); break;
		case GRASS: str.append("Grass"); break;
		case ICE: str.append("Ice"); break;
		case FIGHTING: str.append("Fighting"); break;
		case POISON: str.append("Poison"); break;
		case GROUND: str.append("Ground"); break;
		case PSYCHIC: str.append("Psychic"); break;
		case FLYING: str.append("Flying"); break;
		case BUG: str.append("Bug"); break;
		case ROCK: str.append("Rock"); break;
		case GHOST: str.append("Ghost"); break;
		case DRAGON: str.append("Dragon"); break;
		case DARK: str.append("Dark"); break;
		case STEEL: str.append("Steel"); break;
		case FAIRY: str.append("Fairy"); break;
		}
		if (!i) 	str.append("/");
	}
	return str;
}

std::string TypeManager::DamageToString(DamageResult type)
{
	switch (type) {
	case ZERO: return "0x"; break;
	case QUARTER: return "0.25x"; break;
	case HALF: return "0.5x"; break;
	case ONE: return "1x"; break;
	case DOUBLE: return "2x"; break;
	case QUADRUPLE: return "4x"; break;
	}
	return std::string();
}

void TypeManager::AnalyseTypes()
{
	for (auto& t : types) {
		t.Analyse();
	}
	baseTypesByOffence = types;
	SortTypesByOffense(baseTypesByOffence);

	baseTypesByDefence = types;
	SortTypesByDefense(baseTypesByDefence);

	baseTypesByTotal = types;
	SortTypesByTotal(baseTypesByTotal);
}

void TypeManager::Summary(bool dualType)
{
	std::ofstream outFile;
	if(dualType) outFile.open("dual_type_summary.csv");
	else outFile.open("single_type_summary.csv");
	if (!outFile) {
		std::cout << "Cannot Create File";
		return;
	}
	int whiteSpace = dualType ? 40 : 25;
	for (int i = 0; i < 3; ++i) {
		std::string title = !i ? "AVERAGE DAMAGE DEALT" : i == 1 ? "AVERAGE DAMAGE TAKEN" : "OVERALL SCORE";
		std::cout << std::setw(whiteSpace) << title << "\t";
		if (i != 2) outFile << title << ",";
		else  outFile << title << "\n";
	}
	std::cout << std::endl;
	int numObjs = dualType ? dualTypes.size() : types.size();
	for (int i = 0; i < numObjs; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (dualType) {
				Type current = !j ? dualTypesByOffence.at(i) : j == 1 ? dualTypesByDefence.at(i) : dualTypesByTotal.at(i);
				float value = !j ? current.GetOffensiveStat() : j == 1 ? current.GetDefensiveStat() : current.GetTotalStat();
				std::cout << std::setw(whiteSpace) << std::string(TypeToString(current.GetTypePair()) + ":" + std::to_string(value)) << "\t";
				if (j != 2) outFile << current << ":" + std::to_string(value) << ",";
				else outFile << current << ":" + std::to_string(value) << "\n";
			}
			else {
				MonoType current = !j ? baseTypesByOffence.at(i) : j == 1 ? baseTypesByDefence.at(i) : baseTypesByTotal.at(i);
				float value = !j ? current.GetOffensiveStat() : j == 1 ? current.GetDefensiveStat() : current.GetTotalStat();
				std::cout << std::setw(whiteSpace) << std::string(TypeToString(current.GetType()) + ":" + std::to_string(value)) << "\t";
				if (j != 2) outFile << current << ":" + std::to_string(value) << ",";
				else outFile << current << ":" + std::to_string(value) << "\n";
			}
		}
		std::cout << std::endl;
	}
	outFile.close();
}

void TypeManager::OutputResults(bool dualType)
{
	int whiteSpace = 30;
	std::ofstream outFile;
	if(dualType) outFile.open("dual_type_breakdown.csv");
	else outFile.open("single_type_breakdown.csv");
	if (!outFile) {
		std::cout << "Cannot Create File";
		return;
	}

	if (dualType) {
		for (auto& t : dualTypes) {
			std::cout << "\n------------------------" << t << "------------------------" << std::endl;
			outFile << "------------------------" << t << "------------------------" << "\n";
			for (int i = 0; i < 2; ++i) {
				std::vector<std::string> effects;
				for (DamageResult s = (DamageResult)0; s != NUMDAMAGES; s = (DamageResult)(s + 1))
				{
					effects.push_back(std::string());
				}

				std::map<std::pair<PokemonType, PokemonType>, DamageResult> map = !i ? t.offensive : t.resistance;
				for (auto const& x : map)
				{
					std::string value = TypeToString(x.first);
					switch (x.second) {
					case ZERO:
						effects.at(0).append(" , " + value);
						break;
					case QUARTER:
						effects.at(1).append(" , " + value);
						break;
					case HALF:
						effects.at(2).append(" , " + value);
						break;
					case ONE:
						effects.at(3).append(" , " + value);
						break;
					case DOUBLE:
						effects.at(4).append(" , " + value);
						break;
					case QUADRUPLE:
						effects.at(5).append(" , " + value);
						break;
					}
				}

				if (!i) {
					std::cout << std::setw(whiteSpace) << "|Attacking|" << std::endl;
					outFile << std::setw(whiteSpace) << "|Attacking|\n";
				}
				else {
					std::cout << "\n" << std::setw(whiteSpace) << "|Defending|" << std::endl;
					outFile << "\n" << std::setw(whiteSpace) << "|Defending|" << "\n";
				}
				std::map<DamageResult, int> damageMap = !i ? t.GetOffensiveOccurences() : t.GetResistanceOccurences();
				for (DamageResult s = (DamageResult)0; s != NUMDAMAGES; s = (DamageResult)(s + 1))
				{
					if (damageMap[s]) {
						std::string info = DamageToString(s) + "[" + std::to_string(damageMap[s]) + "]:";
						std::cout << std::setw(whiteSpace) << info;
						outFile << info;

						std::cout << effects.at(s).substr(3, effects.at(s).size() - 1) << std::endl;
						outFile << effects.at(s) << "\n";
					}
				}
				float stat = !i ? t.GetOffensiveStat() : t.GetDefensiveStat();
				std::cout << std::setw(whiteSpace) << "Total Score: " << stat << std::endl;
				outFile << "Total Score: " << stat << "\n";
			}
			outFile << "\n";
		}
	}
	else {
		for (auto& t : types) {
			std::cout << "\n------------------------" << t << "------------------------" << std::endl;
			outFile << "------------------------" << t << "------------------------" << "\n";
			for (int i = 0; i < 2; ++i) {
				std::vector<std::string> effects;
				for (DamageResult s = (DamageResult)0; s != NUMDAMAGES; s = (DamageResult)(s + 1))
				{
					effects.push_back(std::string());
				}

				std::map<PokemonType, DamageResult> map = !i ? t.offensive : t.resistance;
				for (auto const& x : map)
				{
					std::string value = TypeToString(x.first);
					switch (x.second) {
					case ZERO:
						effects.at(0).append(" , " + value);
						break;
					case HALF:
						effects.at(2).append(" , " + value);
						break;
					case ONE:
						effects.at(3).append(" , " + value);
						break;
					case DOUBLE:
						effects.at(4).append(" , " + value);
						break;
					}
				}

				if (!i) {
					std::cout << std::setw(whiteSpace) << "|Attacking|" << std::endl;
					outFile << std::setw(whiteSpace) << "|Attacking|" << "\n";
				}
				else {
					std::cout << "\n" << std::setw(whiteSpace) << "|Defending|" << std::endl;
					outFile << "\n" << std::setw(whiteSpace) << "|Defending|" << "\n";
				}
				std::map<DamageResult, int> damageMap = !i ? t.GetOffensiveOccurences() : t.GetResistanceOccurences();
				for (DamageResult s = (DamageResult)0; s != NUMDAMAGES; s = (DamageResult)(s + 1))
				{
					if (damageMap[s]) {
						std::string info = DamageToString(s) + "[" + std::to_string(damageMap[s]) + "]:";
						std::cout << std::setw(whiteSpace) << info;
						outFile << info;

						std::cout << effects.at(s).substr(3, effects.at(s).size() - 1) << std::endl;
						outFile << effects.at(s) << "\n";
					}
				}
				float stat = !i ? t.GetOffensiveStat() : t.GetDefensiveStat();
				std::cout << std::setw(whiteSpace) << "Total Score: " << stat << std::endl;
				outFile << "Total Score: " << stat << "\n";
			}
			outFile << "\n";
		}
	}
	outFile.close();
}

void TypeManager::AnalyseDualTypes()
{
	for (auto& currentType : dualTypes) {
		std::pair<MonoType, MonoType> monoTypes = currentType.GetTypePair();
		std::pair<PokemonType, PokemonType> currentTypes(monoTypes.first.GetType(), monoTypes.second.GetType());
		for (auto& other : dualTypes) {
			std::pair<PokemonType, PokemonType> otherTypes(other.GetTypePair().first.GetType(), other.GetTypePair().second.GetType());
			DamageResult typeResult[2]{ZERO, ZERO};
			for (int x = 0; x < 2; ++x) {
				MonoType typeInstance = x ? monoTypes.first : monoTypes.second;
				DamageResult offense1 = typeInstance.offensive.find(otherTypes.first)->second;
				DamageResult offense2 = typeInstance.offensive.find(otherTypes.second)->second;
				if (offense1 == ZERO || offense2 == ZERO) {
					typeResult[x] = ZERO;
					continue;
				}
				else {
					std::pair<DamageResult, DamageResult> offensePair(offense1, offense2);
					if (otherTypes.first == otherTypes.second) {
						switch (offensePair.first) {
						case HALF:
							typeResult[x] = HALF;
							break;
						case ONE:
							typeResult[x] = ONE;
							break;
						case DOUBLE:
							typeResult[x] = DOUBLE;
							break;
						}
					}
					else {
						switch (offensePair.first) {
						case HALF:
							switch (offensePair.second) {
							case HALF:
								typeResult[x] = QUARTER;
								break;
							case ONE:
								typeResult[x] = HALF;
								break;
							case DOUBLE:
								typeResult[x] = ONE;
								break;
							}
							break;
						case ONE:
							switch (offensePair.second) {
							case HALF:
								typeResult[x] = HALF;
								break;
							case ONE:
								typeResult[x] = ONE;
								break;
							case DOUBLE:
								typeResult[x] = DOUBLE;
								break;
							}
							break;
						case DOUBLE:
							switch (offensePair.second) {
							case HALF:
								typeResult[x] = ONE;
								break;
							case ONE:
								typeResult[x] = DOUBLE;
								break;
							case DOUBLE:
								typeResult[x] = QUADRUPLE;
								break;
							}
							break;
						}
						if (currentTypes.first == currentTypes.second) break;
					}
				}
			}
			DamageResult bestType = typeResult[1] > typeResult[0] ? typeResult[1] : typeResult[0];
			currentType.offensive[otherTypes] = bestType;
			other.resistance[currentTypes] = bestType;
		}
	}

	for (auto& t : dualTypes) {
		t.Analyse();
	}

	dualTypesByOffence = dualTypes;
	SortTypesByOffense(dualTypesByOffence);

	dualTypesByDefence = dualTypes;
	SortTypesByDefense(dualTypesByDefence);

	dualTypesByTotal = dualTypes;
	SortTypesByTotal(dualTypesByTotal);
}

void TypeManager::SortTypesByOffense(std::vector<MonoType>& sort)
{
	std::sort(sort.begin(), sort.end(), [](const MonoType& lhs, const MonoType& rhs) {
		return lhs.GetOffensiveStat() > rhs.GetOffensiveStat();
	});
}

void TypeManager::SortTypesByOffense(std::vector<Type>& sort)
{
	std::sort(sort.begin(), sort.end(), [](const Type& lhs, const Type& rhs) {
		return lhs.GetOffensiveStat() > rhs.GetOffensiveStat();
		});
}

void TypeManager::SortTypesByDefense(std::vector<MonoType>& sort)
{
	std::sort(sort.begin(), sort.end(), [](const MonoType& lhs, const MonoType& rhs) {
		return lhs.GetDefensiveStat() < rhs.GetDefensiveStat();
	});
}

void TypeManager::SortTypesByDefense(std::vector<Type>& sort)
{
	std::sort(sort.begin(), sort.end(), [](const Type& lhs, const Type& rhs) {
		return lhs.GetDefensiveStat() < rhs.GetDefensiveStat();
		});
}

void TypeManager::SortTypesByTotal(std::vector<MonoType>& sort)
{
	std::sort(sort.begin(), sort.end());
}

void TypeManager::SortTypesByTotal(std::vector<Type>& sort)
{
	std::sort(sort.begin(), sort.end());
}




#pragma once
#include "MonoType.h"
#include <utility>

class Type : public BaseType
{
public:
	Type() {}
	Type(std::pair<MonoType, MonoType> t);
	void Analyse();


	std::pair<MonoType, MonoType> GetType() const {
		return type;
	}

	bool operator==(Type& other) const
	{
		if (type.first.GetType() == other.GetType().first.GetType() && type.second.GetType() == other.GetType().second.GetType()) {
			return true;
		}
		return false;
	}

	friend std::ostream& operator<< (std::ostream& out, const Type& point);

	std::map<std::pair<PokemonType, PokemonType>, DamageResult> offensive;
	std::map<std::pair<PokemonType, PokemonType>, DamageResult> resistance;

private:
	std::pair<MonoType, MonoType> type;
};


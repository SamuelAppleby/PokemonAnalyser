/*		 Created By Samuel Buzz Appleby
 *               12/04/2021
 *			    TypeDefinition			    */
#pragma once
#include "BaseType.h"
class MonoType : public BaseType
{
public:
	MonoType(){}
	MonoType(PokemonType t);
	void Analyse();
	PokemonType GetType() const {
		return type;
	}
	friend std::ostream& operator<< (std::ostream& out, const MonoType& point);

	std::map<PokemonType, DamageResult> offensive;
	std::map<PokemonType, DamageResult> resistance;

private:
	PokemonType type;
};


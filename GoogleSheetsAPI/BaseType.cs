using System;
using System.Collections.Generic;

public enum PokemonType
{
	NORMAL,
	FIRE,
	WATER,
	ELECTRIC,
	GRASS,
	ICE,
	FIGHTING,
	POISON,
	GROUND,
	FLYING,
	PSYCHIC,
	BUG,
	ROCK,
	GHOST,
	DRAGON,
	DARK,
	STEEL,
	FAIRY,
	NUMTYPES
}

public class BaseType : IComparable<BaseType>
{
	public BaseType()
	{
	}

	public int CompareTo(BaseType other)
	{
		if (other == null)
			return 1;
		if (totalStat == other.totalStat)
		{
			return 0;
		}
		else if (totalStat > other.totalStat)
			return -1;
		else
			return 1;
	}

	public static bool operator <(BaseType left, BaseType right)
    {
		return left.CompareTo(right) < 0;
	}

	public static bool operator >(BaseType left, BaseType right)
	{
		return left.CompareTo(right) > 0;
	}

	public Dictionary<float, int> offensiveOccurence = new Dictionary<float, int>();
	public Dictionary<float, int> resistanceOccurence = new Dictionary<float, int>();
	public float offensiveStat { get; set; }
	public float defensiveStat { get; set; }
	public float totalStat { get; set; }
}


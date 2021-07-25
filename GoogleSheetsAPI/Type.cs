using System;
using System.Collections.Generic;

public class Type : BaseType
{
	public Type()
	{
	}
	public Type(Tuple<MonoType, MonoType> t)
	{
		typePair = t;
		offensiveStat = 0;
		defensiveStat = 0;
		totalStat = 0;
	}

	public void Analyse()
	{
		for (int i = 0; i < 2; ++i)
		{
			float currentStat = 0;
			Dictionary<Tuple<PokemonType, PokemonType>, float> map = i == 0 ? offensive : resistance;
			Dictionary<float, int> damageMap = i == 0 ? offensiveOccurence : resistanceOccurence;

			foreach (KeyValuePair<Tuple<PokemonType, PokemonType>, float> entry in map)
			{
				switch (entry.Value)
				{
				case 0F:
					break;
				case 0.25F:
					currentStat += 0.25f;
					break;
				case 0.5F:
					currentStat += 0.5f;
					break;
				case 1F:
					currentStat += 1;
					break;
				case 2F:
					currentStat += 2;
					break;
				case 4F:
					currentStat += 4;
					break;
				}
				if (damageMap.ContainsKey(entry.Value))
				{
					damageMap[entry.Value]++;
				}
				else
				{
					damageMap[entry.Value] = 1;
				}
			}

			currentStat /= map.Count;

			if (i == 0)
            {
				offensiveStat = currentStat;
            }
            else
            {
				defensiveStat = currentStat;
            }
		}
		totalStat = (offensiveStat + (1 / defensiveStat)) / 2;
	}

	public static bool operator ==(Type left, Type right)
	{
		if ((object)left == null)
			return (object)right == null;

		return left.Equals(right);
	}

	public static bool operator !=(Type left, Type right)
	{
		return !(left == right);
	}

	public override bool Equals(object obj)
	{
		if (obj == null || GetType() != obj.GetType())
			return false;

		var right = (Type)obj;
		return (typePair.Item1.type == right.typePair.Item1.type && typePair.Item2.type == right.typePair.Item2.type);
	}

	public override int GetHashCode()
	{
		return typePair.Item1.GetHashCode() ^ typePair.Item2.GetHashCode();
	}

	public override string ToString()
	{
		string strm = "";
		for (int i = 0; i < 2; ++i) {
			PokemonType type = i == 0 ? typePair.Item1.type : typePair.Item2.type;
			strm += type.ToString();
			if (i == 0) strm += "/";
		}
		return strm;
	}

	public Dictionary<Tuple<PokemonType, PokemonType>, float> offensive = new Dictionary<Tuple<PokemonType, PokemonType>, float>();
	public Dictionary<Tuple<PokemonType, PokemonType>, float> resistance = new Dictionary<Tuple<PokemonType, PokemonType>, float>();

	public Tuple<MonoType, MonoType> typePair { get; set; } = new Tuple<MonoType, MonoType>(null, null);
}
using System.Collections.Generic;
using System.Linq;

public class MonoType : BaseType
{
    public MonoType()
    {
    }

    public MonoType(PokemonType t)
    {
        type = t;
        offensiveStat = 0F;
        defensiveStat = 0F;
        totalStat = 0F;
    }

    public void Analyse()
    {
        for (int i = 0; i < 2; ++i)
        {
            float currentStat = 0F;
            Dictionary<PokemonType, float> map = i == 0 ? offensive : resistance;
            Dictionary<float, int> damageMap = i == 0 ? ref offensiveOccurence :ref resistanceOccurence;

            foreach(KeyValuePair<PokemonType, float> entry in map)
            {
                switch (entry.Value)
                {
                    case 0F:
                        break;
                    case .5F:
                        currentStat += 0.5f;
                        break;
                    case 1F:
                        currentStat += 1;
                        break;
                    case 2F:
					    currentStat += 2;
                        break;
                }
                if(damageMap.ContainsKey(entry.Value))
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

    public void SortTypeSimilarities()
    {
        offensiveSimilarities = offensiveSimilarities.OrderByDescending(pair => pair.Value).ToDictionary(pair => pair.Key, pair => pair.Value);
        defensiveSimilarities = defensiveSimilarities.OrderByDescending(pair => pair.Value).ToDictionary(pair => pair.Key, pair => pair.Value);
        overallSimilarities = overallSimilarities.OrderByDescending(pair => pair.Value).ToDictionary(pair => pair.Key, pair => pair.Value);
    }

    public override string ToString()
    {
        string builder = type.ToString();
        return builder;
    }

    public Dictionary<PokemonType, float> offensive { get; set; } = new Dictionary<PokemonType, float>();
    public Dictionary<PokemonType, float> resistance { get; set; } = new Dictionary<PokemonType, float>();
    public Dictionary<PokemonType, float> offensiveSimilarities { get; set; } = new Dictionary<PokemonType, float>();
    public Dictionary<PokemonType, float> defensiveSimilarities { get; set; } = new Dictionary<PokemonType, float>();
    public Dictionary<PokemonType, float> overallSimilarities { get; set; } = new Dictionary<PokemonType, float>();

    public PokemonType type { get; set; }
}

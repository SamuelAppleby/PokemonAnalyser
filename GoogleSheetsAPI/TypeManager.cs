using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

public class TypeManager
{
    public TypeManager(StreamReader stream)
    {
        string line;
        int lineNo = 0;
        char delimiter = ',';

        while (stream.Peek() > -1)
        {
            line = stream.ReadLine();
            Regex.Replace(line, @"\s+", "");

            if (lineNo == 0)
            {
                string[] typeArr = line.Split(delimiter);

                foreach (string type in typeArr)
                {
                    if (type != "")
                    {
                        MonoType t = new MonoType(ConvertTypeName(type));
                        types.Add(t);
                    }
                }

            }
            else
            {
                int tokenNo = 0;
                MonoType currentType = null;

                string[] resultArr = line.Split(delimiter);

                foreach (string result in resultArr)
                {
                    if (tokenNo == 0)
                    {
                        currentType = FindTypeInList(ConvertTypeName(result));
                    }
                    else
                    {
                        PokemonType other = (PokemonType)(tokenNo - 1);
                        MonoType otherType = FindTypeInList(other);
                        float damage = float.Parse(result);
                        currentType.offensive[other] = damage;
                        otherType.resistance[currentType.type] = damage;
                    }
                    tokenNo++;
                }
            }
            ++lineNo;
        }

        for (int i = 0; i < types.Count; i++)
        {
            for (int j = i; j < types.Count; j++)
            {
                Type t = new Type(new Tuple<MonoType, MonoType>(types[i], types[j]));
                dualTypes.Add(t);
            }
        }

        foreach (var currentType in dualTypes)
        {
            Tuple<MonoType, MonoType> monoTypes = currentType.typePair;
            Tuple<PokemonType, PokemonType> currentTypes = new Tuple<PokemonType, PokemonType>(monoTypes.Item1.type, monoTypes.Item2.type);
            foreach (var other in dualTypes)
            {
                Tuple<PokemonType, PokemonType> otherTypes = new Tuple<PokemonType, PokemonType>(other.typePair.Item1.type, other.typePair.Item2.type);
                float[] typeResult = { 0, 0 };
                for (int x = 0; x < 2; ++x)
                {
                    MonoType typeInstance = x != 0 ? monoTypes.Item1 : monoTypes.Item2;
                    float offense1 = typeInstance.offensive[otherTypes.Item1];
                    float offense2 = typeInstance.offensive[otherTypes.Item2];

                    if (offense1 == 0F || offense2 == 0F)
                    {
                        typeResult[x] = 0F;
                        continue;
                    }
                    else
                    {
                        Tuple<float, float> offensePair = new Tuple<float, float>(offense1, offense2);
                        if (otherTypes.Item1 == otherTypes.Item2)
                        {
                            switch (offensePair.Item1)
                            {
                                case 0.5F:
                                    typeResult[x] = 0.5F;
                                    break;
                                case 1F:
                                    typeResult[x] = 1F;
                                    break;
                                case 2F:
                                    typeResult[x] = 2F;
                                    break;
                            }
                        }
                        else
                        {
                            switch (offensePair.Item1)
                            {
                                case 0.5F:
                                    switch (offensePair.Item2)
                                    {
                                        case 0.5F:
                                            typeResult[x] = 0.25F;
                                            break;
                                        case 1F:
                                            typeResult[x] = 0.5F;
                                            break;
                                        case 2F:
                                            typeResult[x] = 1F;
                                            break;
                                    }
                                    break;
                                case 1F:
                                    switch (offensePair.Item2)
                                    {
                                        case 0.5F:
                                            typeResult[x] = 0.5F;
                                            break;
                                        case 1F:
                                            typeResult[x] = 1F;
                                            break;
                                        case 2F:
                                            typeResult[x] = 2F;
                                            break;
                                    }
                                    break;
                                case 2F:
                                    switch (offensePair.Item2)
                                    {
                                        case 0.5F:
                                            typeResult[x] = 1F;
                                            break;
                                        case 1F:
                                            typeResult[x] = 2F;
                                            break;
                                        case 2F:
                                            typeResult[x] = 4F;
                                            break;
                                    }
                                    break;
                            }
                            if (currentTypes.Item1 == currentTypes.Item2)
                            {
                                break;
                            }
                        }
                    }
                }

                float bestType = typeResult[1] > typeResult[0] ? typeResult[1] : typeResult[0];
                currentType.offensive[otherTypes] = bestType;
                other.resistance[currentTypes] = bestType;
            }
        }
    }

    public MonoType FindTypeInList(PokemonType type)
    {
        foreach (MonoType t in types)
        {
            if (type == t.type)
            {
                return t;
            }
        }
        return null;
    }

    public static PokemonType ConvertTypeName(string name)
    {
        PokemonType type = (PokemonType)System.Enum.Parse(typeof(PokemonType), name.ToUpper());
        return type;
    }

    public string TypeToString(Tuple<MonoType, MonoType> types)
    {
        string str = "";
        str += types.Item1.type.ToString();
        str += "/";
        str += types.Item2.type.ToString();
        return str;
    }

    public string TypeToString(Tuple<PokemonType, PokemonType> types)
    {
        string str = "";
        str += types.Item1.ToString();
        str += "/";
        str += types.Item2.ToString();
        return str;
    }

    public void AnalyseTypes()
    {
        foreach (MonoType t in types)
        {
            t.Analyse();
        }

        baseTypesByOffence = new List<MonoType>(types);
        baseTypesByOffence = baseTypesByOffence.OrderBy(o => 1 / o.offensiveStat).ToList();

        baseTypesByDefence = new List<MonoType>(types);
        baseTypesByDefence = baseTypesByDefence.OrderBy(o => o.defensiveStat).ToList();

        baseTypesByTotal = new List<MonoType>(types);
        baseTypesByTotal.Sort();

        foreach (Type t in dualTypes)
        {
            t.Analyse();
        }

        dualTypesByOffence = new List<Type>(dualTypes);
        dualTypesByOffence = dualTypesByOffence.OrderBy(o => 1 / o.offensiveStat).ToList();

        dualTypesByDefence = new List<Type>(dualTypes);
        dualTypesByDefence = dualTypesByDefence.OrderBy(o => o.defensiveStat).ToList();

        dualTypesByTotal = new List<Type>(dualTypes);
        dualTypesByTotal.Sort();
    }

    public void ComputeTypeSimilarities()
    {
        for (int i = 0; i < types.Count; ++i)
        {
            MonoType first = types[i];
            for (int j = i + 1; j < types.Count; ++j)
            {
                MonoType second = types[j];

                for (int k = 0; k < 2; ++k)
                {
                    float currentSimilarity = 0F;
                    Dictionary<PokemonType, float> firstMap = k == 0 ? first.offensive : first.resistance;
                    Dictionary<PokemonType, float> secondMap = k == 0 ? second.offensive : second.resistance;

                    foreach (KeyValuePair<PokemonType, float> entry in firstMap)
                    {
                        float secondValue = secondMap[entry.Key];

                        float difference = Math.Abs(entry.Value - secondValue);

                        switch (difference)
                        {
                            case 0F:
                                currentSimilarity += 3F;
                                break;
                            case .5F:
                                currentSimilarity += 2F;
                                break;
                            case 1F:
                                currentSimilarity += 1F;
                                break;
                            case 2F:
                                break;
                        }
                    }

                    currentSimilarity /= (3 * 17);

                    if (k == 0)
                    {
                        first.offensiveSimilarities.Add(second.type, currentSimilarity);
                        second.offensiveSimilarities.Add(first.type, currentSimilarity);
                    }
                    else
                    {
                        first.defensiveSimilarities.Add(second.type, currentSimilarity);
                        second.defensiveSimilarities.Add(first.type, currentSimilarity);
                    }
                }

                float overall = (first.offensiveSimilarities[second.type] + first.defensiveSimilarities[second.type]) / 2F;
                first.overallSimilarities.Add(second.type, overall);
                second.overallSimilarities.Add(first.type, overall);
            }

            first.SortTypeSimilarities();
        }
    }

    public void TypeSimilaritySummary(string filePath)
    {
        var csv = new StringBuilder();
        bool setup = false;
        foreach (MonoType t in types)
        {
            if(setup)
            {
                Console.WriteLine();
                csv.AppendLine();
            }

            setup = true;

            Console.WriteLine("{0, 25}", t.type.ToString());
            csv.AppendLine(t.type.ToString());

            Console.Write("{0, 25}", "DEFENSIVE SIMILARITIES");
            csv.Append("DEFENSIVE SIMILARITIES,,");
            Console.Write("{0, 25}", "OFFENSIVE SIMILARITIES");
            csv.Append("OFFENSIVE SIMILARITIES,,");
            Console.WriteLine("{0, 25}", "OVERALL SIMILARITIES");
            csv.AppendLine("OVERALL SIMILARITIES");

            for (int i = 0; i < 6; ++i)
            {
                if (i%2 == 0)
                {
                    Console.Write("{0, 15}", "TYPE");
                    csv.Append("TYPE,");
                }
                else
                {
                    Console.Write("{0, 10}", "SCORE");
                    csv.Append("SCORE,");
                }
            }

            Console.WriteLine();
            csv.AppendLine();

            for (int i = 0; i < types.Count - 1; ++i)
            {
                Console.Write("{0, 16}", t.offensiveSimilarities.ElementAt(i).Key.ToString() + ": ");
                Console.Write("{0, 9}", String.Format("{0:0.0}", t.offensiveSimilarities.ElementAt(i).Value * 100) + "%");
                csv.Append(t.offensiveSimilarities.ElementAt(i).Key.ToString() + ": ,"
                    + String.Format("{0:0.0}", t.offensiveSimilarities.ElementAt(i).Value * 100) + "%,");

                Console.Write("{0, 16}", t.defensiveSimilarities.ElementAt(i).Key.ToString() + ": ");
                Console.Write("{0, 9}", String.Format("{0:0.0}", t.defensiveSimilarities.ElementAt(i).Value * 100) + "%");
                csv.Append(t.defensiveSimilarities.ElementAt(i).Key.ToString() + ": ,"
                    + String.Format("{0:0.0}", t.defensiveSimilarities.ElementAt(i).Value * 100) + "%,");

                Console.Write("{0, 16}", t.overallSimilarities.ElementAt(i).Key.ToString() + ": ");
                Console.WriteLine("{0, 9}", String.Format("{0:0.0}", t.overallSimilarities.ElementAt(i).Value * 100) + "%");
                csv.AppendLine(t.overallSimilarities.ElementAt(i).Key.ToString() + ": ,"
                    + String.Format("{0:0.0}", t.overallSimilarities.ElementAt(i).Value * 100) + "%");
            }
        }


        File.WriteAllText(filePath, csv.ToString());
    }

    public void TypeScoreSummary(string filePath, bool singleType = false)
    {
        var csv = new StringBuilder();
        char delimiter = ',';

        for (int i = 0; i < 6; ++i)
        {
            string title = i%2 == 0 ? "TYPE" : i == 1 ? "AVERAGE DAMAGE DEALT" : i == 3 ? "AVERAGE DAMAGE TAKEN" : "OVERALL SCORE";

            Console.Write("{0, 25}", title);

            if (i != 5)
            {
                csv.Append(title + delimiter);
            }
            else
            {
                csv.AppendLine(title);
            }
        }

        Console.WriteLine();

        int numObjs = singleType ? types.Count : dualTypes.Count;

        for (int i = 0; i < numObjs; ++i)
        {
            for (int j = 0; j < 6; ++j)
            {
                BaseType current;

                if (singleType)
                {
                    current = j == 0 || j == 1 ? baseTypesByOffence[i] : j == 2 || j == 3 ? baseTypesByDefence[i] : baseTypesByTotal[i];
                }

                else
                {
                    current = j == 0 || j == 1 ? dualTypesByOffence[i] : j == 2 || j == 3 ? dualTypesByDefence[i] : dualTypesByTotal[i];
                }

                if (j == 0 || j == 2 || j == 4)
                {
                    if (singleType)
                    {
                        Console.Write("{0, 25}", ((MonoType)current).type);
                        csv.Append(((MonoType)current).type.ToString() + delimiter);
                    }

                    else
                    {
                        Console.Write("{0, 25}", TypeToString(((Type)current).typePair));
                        csv.Append(TypeToString(((Type)current).typePair) + delimiter);
                    }

                }

                else
                {
                    float value = j == 1 ? current.offensiveStat : j == 3 ? current.defensiveStat : current.totalStat;
                    Console.Write("{0, 25}", (String.Format("{0:0.000}", value)));
                    if (j != 5)
                    {
                        csv.Append(Convert.ToString(value) + delimiter);
                    }
                    else
                    {
                        csv.AppendLine(Convert.ToString(value));
                    }
                }
            }

            Console.WriteLine();
        }

        File.WriteAllText(filePath, csv.ToString());
    }

    public void BreakdownResults(string filePath, bool singleType = false)
    {
        var csv = new StringBuilder();

        if (singleType)
        {
            foreach (var t in types)
            {
                Console.WriteLine("------------------------" + t + "------------------------");
                csv.AppendLine("------------------------" + t + "------------------------");

                for (int i = 0; i < 2; ++i)
                {
                    List<string> effects = new List<string>();
                    float[] damages = { 0F, 0.5F, 1F, 2F };
                    Dictionary<float, int> damageMap = i == 0 ? t.offensiveOccurence : t.resistanceOccurence;

                    foreach (float s in damages)
                    {
                        string str = damageMap.ContainsKey(s) ? Convert.ToString(damageMap[s]) : "0";
                        effects.Add(s.ToString() + "x" + "[" + str + "]: ");
                    }

                    Dictionary<PokemonType, float> map = i == 0 ? t.offensive : t.resistance;

                    foreach (KeyValuePair<PokemonType, float> entry in map)
                    {
                        string value = entry.Key.ToString();
                        switch (entry.Value)
                        {
                            case 0F:
                                effects[0] += (", " + value);
                                break;
                            case 0.5F:
                                effects[1] += (", " + value);
                                break;
                            case 1F:
                                effects[2] += (", " + value);
                                break;
                            case 2F:
                                effects[3] += (", " + value);
                                break;
                        }
                    }

                    if (i == 0)
                    {
                        Console.WriteLine("|Attacking|");
                        csv.AppendLine("|Attacking|");
                    }
                    else
                    {
                        Console.WriteLine("|Defending|");
                        csv.AppendLine("|Defending|");
                    }

                    foreach (string s in effects)
                    {
                        Console.WriteLine(s);
                        csv.AppendLine(s);
                    }

                    float stat = i == 0 ? t.offensiveStat : t.defensiveStat;
                    Console.WriteLine("Total Score: " + String.Format("{0:0.000}", stat));
                    csv.AppendLine("Total Score: " + String.Format("{0:0.000}", stat));
                }
                csv.AppendLine();
            }
        }
        else
        {
            foreach (var t in dualTypes)
            {
                Console.WriteLine("------------------------" + t + "------------------------" + "");
                csv.AppendLine("------------------------" + t + "------------------------");

                for (int i = 0; i < 2; ++i)
                {
                    List<string> effects = new List<string>();

                    float[] damages = { 0F, 0.25F, 0.5F, 1F, 2F, 4F };
                    Dictionary<float, int> damageMap = i == 0 ? t.offensiveOccurence : t.resistanceOccurence;

                    foreach (float s in damages)
                    {
                        string str = damageMap.ContainsKey(s) ? Convert.ToString(damageMap[s]) : "0";
                        effects.Add(s.ToString() + "x" + "[" + str + "]: ");
                    }

                    Dictionary<Tuple<PokemonType, PokemonType>, float> map = i == 0 ? t.offensive : t.resistance;

                    foreach (KeyValuePair<Tuple<PokemonType, PokemonType>, float> entry in map)
                    {
                        string value = TypeToString(entry.Key);
                        switch (entry.Value)
                        {
                            case 0F:
                                effects[0] += (", " + value);
                                break;
                            case 0.25F:
                                effects[1] += (", " + value);
                                break;
                            case 0.5F:
                                effects[2] += (", " + value);
                                break;
                            case 1F:
                                effects[3] += (", " + value);
                                break;
                            case 2F:
                                effects[4] += (", " + value);
                                break;
                            case 4F:
                                effects[5] += (", " + value);
                                break;
                        }
                    }

                    if (i == 0)
                    {
                        Console.WriteLine("|Attacking|");
                        csv.AppendLine("|Attacking|");
                    }
                    else
                    {
                        Console.WriteLine("|Defending|");
                        csv.AppendLine("|Defending|");
                    }

                    foreach (string s in effects)
                    {
                        Console.WriteLine(s);
                        csv.AppendLine(s);
                    }

                    float stat = i == 0 ? t.offensiveStat : t.defensiveStat;
                    Console.WriteLine("{0}", "Total Score: " + stat);
                    csv.AppendLine("Total Score: " + stat);
                }
                csv.AppendLine();
            }
        }

        File.WriteAllText(filePath, csv.ToString());
    }

    public List<MonoType> types { get; set; } = new List<MonoType>();

    public List<MonoType> baseTypesByOffence { get; set; }
    public List<MonoType> baseTypesByDefence { get; set; }
    public List<MonoType> baseTypesByTotal { get; set; }

    public List<Type> dualTypes { get; set; } = new List<Type>();
    public List<Type> dualTypesByOffence { get; set; }
    public List<Type> dualTypesByDefence { get; set; }
    public List<Type> dualTypesByTotal { get; set; }

}
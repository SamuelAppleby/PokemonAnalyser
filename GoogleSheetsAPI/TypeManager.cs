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

                    currentSimilarity /= ((3 * (types.Count - 1)));
                    currentSimilarity *= 100F;

                    if (k == 0)
                    {
                        first.offensiveSimilarities.Add(second.type, currentSimilarity);
                        second.offensiveSimilarities.Add(first.type, currentSimilarity);

                        similaritiesByOffense.Add(Tuple.Create(first.type, second.type), currentSimilarity);
                    }
                    else
                    {
                        first.defensiveSimilarities.Add(second.type, currentSimilarity);
                        second.defensiveSimilarities.Add(first.type, currentSimilarity);

                        similaritiesByDefence.Add(Tuple.Create(first.type, second.type), currentSimilarity);
                    }
                }

                float overall = (first.offensiveSimilarities[second.type] + first.defensiveSimilarities[second.type]) / 2F;
                first.overallSimilarities.Add(second.type, overall);
                second.overallSimilarities.Add(first.type, overall);

                similaritiesByTotal.Add(Tuple.Create(first.type, second.type), overall);
            }

            first.SortTypeSimilarities();

        }

        SortSimilarityTables();
    }

    public void SortSimilarityTables()
    {
        similaritiesByOffense = similaritiesByOffense.OrderByDescending(pair => pair.Value).ToDictionary(pair => pair.Key, pair => pair.Value);
        similaritiesByDefence = similaritiesByDefence.OrderByDescending(pair => pair.Value).ToDictionary(pair => pair.Key, pair => pair.Value);
        similaritiesByTotal = similaritiesByTotal.OrderByDescending(pair => pair.Value).ToDictionary(pair => pair.Key, pair => pair.Value);
    }

    public void TypeSimilaritySummary(string filePath)
    {
        var csv = new StringBuilder();
        bool setup = false;
        foreach (MonoType t in types)
        {
            if (setup)
            {
                Console.WriteLine();
                csv.AppendLine();
            }

            setup = true;

            Console.WriteLine("{0, 40}", t.type.ToString());
            csv.AppendLine(t.type.ToString());

            Console.Write("{0, 40}", "DEFENSIVE SIMILARITIES");
            csv.Append("DEFENSIVE SIMILARITIES,,,");
            Console.Write("{0, 40}", "OFFENSIVE SIMILARITIES");
            csv.Append("OFFENSIVE SIMILARITIES,,,");
            Console.WriteLine("{0, 40}", "OVERALL SIMILARITIES");
            csv.AppendLine("OVERALL SIMILARITIES");

            for (int i = 0; i < 3; ++i)
            {
                Console.Write("{0, 20}", "TYPE");
                Console.Write("{0, 20}", "SCORE");

                if (i != 2)
                {
                    csv.Append("TYPE, SCORE,,");
                }
                else
                {
                    csv.AppendLine("TYPE, SCORE");
                }
            }

            Console.WriteLine();

            for (int i = 0; i < t.overallSimilarities.Count - 1; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    KeyValuePair<PokemonType, float> elem = j == 0 ? t.offensiveSimilarities.ElementAt(i) :
                        j == 1 ? t.defensiveSimilarities.ElementAt(i) : t.overallSimilarities.ElementAt(i);

                    Console.Write("{0, 20}", elem.Key.ToString() + ":");
                    Console.Write("{0, 20}", String.Format("{0:0.0}", elem.Value) + "%");
                    csv.Append(elem.Key.ToString() + ": ," + String.Format("{0:0.0}", elem.Value) + "%,,");

                    if (j == 2)
                    {
                        Console.WriteLine();
                        csv.AppendLine();
                    }
                }
            }
        }
        File.WriteAllText(filePath, csv.ToString());
    }

    public void TypeSimilarityTable(string filePath)
    {
        var csv = new StringBuilder();
        char delimiter = ',';

        for (int i = 0; i < 3; ++i)
        {
            string title = i == 0 ? "OFFENSIVE SIMILARITIES" : i == 1 ? "DEFENSIVE SIMILARITIES" : "OVERALL SIMILARITIES";

            Console.Write("{0, 40}", title);

            if (i != 2)
            {
                csv.Append(title + delimiter + delimiter + delimiter);
            }
            else
            {
                csv.AppendLine(title);
            }
        }

        Console.WriteLine();

        for (int i = 0; i < 3; ++i)
        {
            Console.Write("{0, 20}", "TYPE");
            Console.Write("{0, 20}", "SCORE");

            if (i != 2)
            {
                csv.Append("TYPE, SCORE,,");
            }
            else
            {
                csv.AppendLine("TYPE, SCORE");
            }
        }

        Console.WriteLine();

        for (int i = 0; i < similaritiesByTotal.Count; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                KeyValuePair<Tuple<PokemonType, PokemonType>, float> elem = j == 0 ? similaritiesByOffense.ElementAt(i) :
                    j == 1 ? similaritiesByDefence.ElementAt(i) : similaritiesByTotal.ElementAt(i);

                Console.Write("{0, 20}", TypeToString(elem.Key) + ":");
                Console.Write("{0, 20}", String.Format("{0:0.0}", elem.Value) + "%");
                csv.Append(TypeToString(elem.Key) + ": ," + String.Format("{0:0.0}", elem.Value) + "%,,");

                if(j == 2)
                {
                    Console.WriteLine();
                    csv.AppendLine();
                }
            }            
        }

        File.WriteAllText(filePath, csv.ToString());
    }

    public void TypeScoreSummary(string filePath, bool singleType = false)
    {
        var csv = new StringBuilder();
        char delimiter = ',';

        Console.Write("{0, 40}", "AVERAGE DAMAGE DEALT");
        csv.Append("AVERAGE DAMAGE DEALT,,,");
        Console.Write("{0, 40}", "AVERAGE DAMAGE TAKEN");
        csv.Append("AVERAGE DAMAGE TAKEN,,,");
        Console.WriteLine("{0, 40}", "OVERALL SCORE");
        csv.AppendLine("OVERALL SCORE");

        for (int i = 0; i < 3; ++i)
        {
            Console.Write("{0, 20}", "TYPE");
            Console.Write("{0, 20}", "SCORE");

            if (i != 2)
            {
                csv.Append("TYPE, SCORE,,");
            }
            else
            {
                csv.AppendLine("TYPE, SCORE");
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
                    current = j <= 1 ? baseTypesByOffence[i] : j <= 3 ? baseTypesByDefence[i] : baseTypesByTotal[i];
                }

                else
                {
                    current = j <= 1 ? dualTypesByOffence[i] : j <= 3 ? dualTypesByDefence[i] : dualTypesByTotal[i];
                }

                if (j % 2 == 0)
                {
                    string typeName = singleType ? ((MonoType)current).type.ToString() + ":" : TypeToString(((Type)current).typePair) + ":";
                    Console.Write("{0, 20}", typeName);
                    csv.Append(typeName + delimiter);
                }

                else
                {
                    float value = j == 1 ? current.offensiveStat : j == 3 ? current.defensiveStat : current.totalStat;
                    Console.Write("{0, 20}", (String.Format("{0:0.000}", value)));

                    if (j != 5)
                    {
                        csv.Append(Convert.ToString(value) + delimiter + delimiter);
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

        List<BaseType> typeConv = singleType ? types.Cast<BaseType>().ToList() : dualTypes.Cast<BaseType>().ToList();

        foreach (var elem in typeConv)
        {
            Console.WriteLine("------------------------" + elem + "------------------------" + "");
            csv.AppendLine("------------------------" + elem + "------------------------");

            float[] damages = { 0F, 0.25F, 0.5F, 1F, 2F, 4F };
            string[] effects = new string[damages.Length];

            for (int i = 0; i < 2; ++i)
            {
                string title = i == 0 ? "|Attacking|" : "|Defending|";
                Console.WriteLine(title);
                csv.AppendLine(title);

                Dictionary<float, int> damageMap = i == 0 ? elem.offensiveOccurence : elem.resistanceOccurence;

                int pos = 0;
                foreach (float s in damages)
                {
                    string str = damageMap.ContainsKey(s) ? Convert.ToString(damageMap[s]) : "0";
                    effects[pos] = s.ToString() + "x" + "[" + str + "]: ";
                    ++pos;
                }

                bool[] first = { true, true, true, true, true, true };

                if (singleType)
                {
                    Dictionary<PokemonType, float> map = i == 0 ? ((MonoType)elem).offensive : ((MonoType)elem).resistance;

                    foreach (KeyValuePair<PokemonType, float> entry in map)
                    {
                        AddEffectOntoString(entry.Value, entry.Key.ToString(), effects, first);
                    }
                }

                else
                {
                    Dictionary<Tuple<PokemonType, PokemonType>, float> map = i == 0 ? ((Type)elem).offensive : ((Type)elem).resistance;

                    foreach (KeyValuePair<Tuple<PokemonType, PokemonType>, float> entry in map)
                    {
                        AddEffectOntoString(entry.Value, TypeToString(entry.Key), effects, first);                      
                    }
                }

                foreach (string consoleString in effects)
                {
                    Console.WriteLine(consoleString);
                    int stringPos = consoleString.IndexOf(':');
                    string csvString = consoleString.Insert(++stringPos, ",");
                    csv.AppendLine(csvString);
                }

                float stat = i == 0 ? elem.offensiveStat : elem.defensiveStat;
                Console.WriteLine("{0}", "Total Score: " + stat);
                csv.AppendLine("Total Score:," + stat);

            }

            csv.AppendLine();
        }
    
        File.WriteAllText(filePath, csv.ToString());
    }

    public void AddEffectOntoString(float damage, string value, string[] effects, bool[] first)
    {
        switch (damage)
        {
            case 0F:
                effects[0] += first[0] ? value : ", " + value;
                first[0] = false;
                break;
            case 0.25F:
                effects[1] += first[1] ? value : ", " + value;
                first[1] = false;
                break;
            case 0.5F:
                effects[2] += first[2] ? value : ", " + value;
                first[2] = false;
                break;
            case 1F:
                effects[3] += first[3] ? value : ", " + value;
                first[3] = false;
                break;
            case 2F:
                effects[4] += first[4] ? value : ", " + value;
                first[4] = false;
                break;
            case 4F:
                effects[5] += first[5] ? value : value + ", ";
                first[5] = false;
                break;
        }
    }

    public List<MonoType> types { get; set; } = new List<MonoType>();

    public List<MonoType> baseTypesByOffence { get; set; }
    public List<MonoType> baseTypesByDefence { get; set; }
    public List<MonoType> baseTypesByTotal { get; set; }

    public List<Type> dualTypes { get; set; } = new List<Type>();
    public List<Type> dualTypesByOffence { get; set; }
    public List<Type> dualTypesByDefence { get; set; }
    public List<Type> dualTypesByTotal { get; set; }

    public Dictionary<Tuple<PokemonType, PokemonType>, float> similaritiesByOffense = new Dictionary<Tuple<PokemonType, PokemonType>, float>();
    public Dictionary<Tuple<PokemonType, PokemonType>, float> similaritiesByDefence = new Dictionary<Tuple<PokemonType, PokemonType>, float>();

    public Dictionary<Tuple<PokemonType, PokemonType>, float> similaritiesByTotal = new Dictionary<Tuple<PokemonType, PokemonType>, float>();

}
using Google.Apis.Sheets.v4;
using System;
using System.Collections.Generic;
using System.IO;

class Program
{
    const string FILE_DIRECTORY = "C:\\Users\\sambu\\source\\repos\\PokemonAnalyser\\GoogleSheetsAPI\\";
    public static StreamReader LoadFile(string FilePath)
    {
        StreamReader infile = new StreamReader(FilePath);
        if (infile == null)
        {
            Console.Write(FilePath);
            Console.Write("Does Not Exist");
        }
        return infile;
    }

    public static char UserInput()
    {
        string input;
        input = Console.ReadLine();
        input = input.ToLower();
        while (input.Length != 1 || (input[0] != 'y' && input[0] != 'n'))
        {
            Console.Write("Please Enter Y or N: ");
            input = Console.ReadLine();
            input = input.ToLower();
        }
        return input[0];
    }

    public static void WriteToGoogleSheet(string FilePath, string GoogleSheetName)
    {
        Console.Write("Writing to Google Sheets...");

        string[] scopes = { SheetsService.Scope.Spreadsheets };

        var gsh = new GoogleSheetsHelper(scopes, "GoogleSheetsAPI",
            "credentials.json", "19ZZgsiHeulJN3ew3-FosjVkF7dIJB-PY3K4l7UwVbhs");

        StreamReader csvStream = LoadFile(FilePath);

        GoogleSheetParameters parameters = new GoogleSheetParameters()
        {
            SheetName = GoogleSheetName,
            RangeRowStart = 1,
            RangeColumnStart = 1
        };

        gsh.WriteData(csvStream, parameters);
        Console.WriteLine("Done!\n");
    }

    static void Main(string[] args)
    {
        Console.WriteLine("Loading Files...");
        StreamReader typeSteam = LoadFile(FILE_DIRECTORY + "typechart.csv");
        if (typeSteam == null)
        {
            return;
        }

        Console.WriteLine("Creating Types...");
        TypeManager manager = new TypeManager(typeSteam);

        Console.WriteLine("Analysing Types...");
        manager.AnalyseTypes();

        Console.WriteLine("Computing Type Similarities...");
        manager.ComputeTypeSimilarities();

        manager.TypeSimilaritySummary(FILE_DIRECTORY + "type_similarities.csv");

        typeSteam.Close();

        WriteToGoogleSheet(FILE_DIRECTORY + "type_similarities.csv", "TypeSimilarities");

        for (int i = 0; i < 2; ++i)
        {
            bool isSingleType = i == 0;

            manager.TypeScoreSummary(FILE_DIRECTORY + (i == 0 ? "single_type_summary.csv" : "dual_type_summary.csv"), isSingleType);

            Console.Write("Would you like a breakdown of the types (Y/N)?: ");

            if (UserInput() == 'y')
            {
                manager.BreakdownResults(FILE_DIRECTORY + (i == 0 ? "single_type_breakdown.csv" : "dual_type_breakdown.csv"), isSingleType);
            }

            string fileName = isSingleType ? "single_type_summary.csv" : "dual_type_summary.csv";
            string sheetName = isSingleType ? "SingleTypeRanking" : "DualTypeRanking";
            WriteToGoogleSheet(FILE_DIRECTORY + fileName, sheetName);
        }
    }
}

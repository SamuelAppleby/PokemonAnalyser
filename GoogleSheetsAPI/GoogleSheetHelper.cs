using Google.Apis.Auth.OAuth2;
using Google.Apis.Sheets.v4;
using Google.Apis.Sheets.v4.Data;
using Google.Apis.Services;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Dynamic;
using System.Text.RegularExpressions;

public class GoogleSheetsHelper
{
    static string[] Scopes;
    static string ApplicationName;
    static string CredentialFileName;
    static string GoogleSheetIdName;
    private readonly SheetsService _sheetsService;

    public GoogleSheetsHelper(string[] scopes, string appName, string credentialName, string sheetId)
    {
        Scopes = scopes;
        CredentialFileName = credentialName;
        ApplicationName = appName;
        GoogleSheetIdName = sheetId;
        var credential = GoogleCredential.FromStream(new FileStream(CredentialFileName, FileMode.Open)).CreateScoped(Scopes);

        _sheetsService = new SheetsService(new BaseClientService.Initializer()
        {
            HttpClientInitializer = credential,
            ApplicationName = ApplicationName,
        });
    }

    public List<ExpandoObject> GetDataFromSheet(GoogleSheetParameters googleSheetParameters)
    {
        googleSheetParameters = MakeGoogleSheetDataRangeColumnsZeroBased(googleSheetParameters);
        var range = $"{googleSheetParameters.SheetName}!{GetColumnName(googleSheetParameters.RangeColumnStart)}{googleSheetParameters.RangeRowStart}:{GetColumnName(googleSheetParameters.RangeColumnEnd)}{googleSheetParameters.RangeRowEnd}";

        SpreadsheetsResource.ValuesResource.GetRequest request =
            _sheetsService.Spreadsheets.Values.Get(GoogleSheetIdName, range);

        var numberOfColumns = googleSheetParameters.RangeColumnEnd - googleSheetParameters.RangeColumnStart;
        var columnNames = new List<string>();
        var returnValues = new List<ExpandoObject>();

        if (!googleSheetParameters.FirstRowIsHeaders)
        {
            for (var i = 0; i <= numberOfColumns; i++)
            {
                columnNames.Add($"Column{i}");
            }
        }

        var response = request.Execute();

        int rowCounter = 0;
        IList<IList<Object>> values = response.Values;
        if (values != null && values.Count > 0)
        {
            foreach (var row in values)
            {
                if (googleSheetParameters.FirstRowIsHeaders && rowCounter == 0)
                {
                    for (var i = 0; i <= numberOfColumns; i++)
                    {
                        columnNames.Add(row[i].ToString());
                    }
                    rowCounter++;
                    continue;
                }

                var expando = new ExpandoObject();
                var expandoDict = expando as IDictionary<String, object>;
                var columnCounter = 0;
                foreach (var columnName in columnNames)
                {
                    expandoDict.Add(columnName, row[columnCounter].ToString());
                    columnCounter++;
                }
                returnValues.Add(expando);
                rowCounter++;
            }
        }

        return returnValues;
    }

    public void AddCells(GoogleSheetParameters googleSheetParameters, List<GoogleSheetRow> rows)
    {
        var requests = new BatchUpdateSpreadsheetRequest { Requests = new List<Request>() };

        var GoogleSheetId = GetSheetId(_sheetsService, GoogleSheetIdName, googleSheetParameters.SheetName);

        GridCoordinate gc = new GridCoordinate
        {
            ColumnIndex = googleSheetParameters.RangeColumnStart - 1,
            RowIndex = googleSheetParameters.RangeRowStart - 1,
            SheetId = GoogleSheetId
        };

        var request = new Request { UpdateCells = new UpdateCellsRequest { Start = gc, Fields = "*" } };

        var listRowData = new List<RowData>();

        foreach (var row in rows)
        {
            var rowData = new RowData();
            var listCellData = new List<CellData>();
            foreach (var cell in row.Cells)
            {
                var cellData = new CellData();
                var extendedValue = new ExtendedValue { StringValue = cell.CellValue };

                cellData.UserEnteredValue = extendedValue;
                var cellFormat = new CellFormat
                {
                    TextFormat = new TextFormat()
                    {
                        Bold = cell.IsBold
                    },

                    HorizontalAlignment = cell.Allignment,

                    BackgroundColor = new Color
                    {
                        Blue = (float)cell.BackgroundColor.B / 255,
                        Red = (float)cell.BackgroundColor.R / 255,
                        Green = (float)cell.BackgroundColor.G / 255
                    }
                };

                cellData.UserEnteredFormat = cellFormat;
                listCellData.Add(cellData);
            }
            rowData.Values = listCellData;
            listRowData.Add(rowData);
        }
        request.UpdateCells.Rows = listRowData;

        requests.Requests.Add(request);

        _sheetsService.Spreadsheets.BatchUpdate(requests, GoogleSheetIdName).Execute();
    }


    public void WriteData(StreamReader stream, GoogleSheetParameters parameters)
    {
        char delimiter = ',';

        var rows = new List<GoogleSheetRow>();

        while (stream.Peek() > -1)
        {
            var row = new GoogleSheetRow();
            string[] names = stream.ReadLine().Split(delimiter);

            List<GoogleSheetCell> cells = new List<GoogleSheetCell>();

            foreach (string type in names)
            {
                cells.Add(new GoogleSheetCell()
                {
                    IsBold = true,
                    CellValue = type,
                    Allignment = "Right",
                    BackgroundColor = type.Length > 0 ? System.Drawing.Color.Azure : System.Drawing.Color.White
                });
            }

            row.Cells.AddRange(cells);

            rows.Add(row);
        }

        AddCells(parameters, rows);
    }

    private string GetColumnName(int index)
    {
        const string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        var value = "";

        if (index >= letters.Length)
            value += letters[index / letters.Length - 1];

        value += letters[index % letters.Length];
        return value;
    }

    private GoogleSheetParameters MakeGoogleSheetDataRangeColumnsZeroBased(GoogleSheetParameters googleSheetParameters)
    {
        googleSheetParameters.RangeColumnStart = googleSheetParameters.RangeColumnStart - 1;
        googleSheetParameters.RangeColumnEnd = googleSheetParameters.RangeColumnEnd - 1;
        return googleSheetParameters;
    }

    private int GetSheetId(SheetsService service, string spreadSheetId, string spreadSheetName)
    {
        var spreadsheet = service.Spreadsheets.Get(spreadSheetId).Execute();
        var sheet = spreadsheet.Sheets.FirstOrDefault(s => s.Properties.Title == spreadSheetName);
        int sheetId = (int)sheet.Properties.SheetId;
        return sheetId;
    }
}

public class GoogleSheetCell
{
    public string CellValue { get; set; }
    public bool IsBold { get; set; }
    public string Allignment { get; set; }
    public System.Drawing.Color BackgroundColor { get; set; } = System.Drawing.Color.Azure;
}

public class GoogleSheetParameters
{
    public int RangeColumnStart { get; set; }
    public int RangeRowStart { get; set; }
    public int RangeColumnEnd { get; set; }
    public int RangeRowEnd { get; set; }
    public string SheetName { get; set; }
    public bool FirstRowIsHeaders { get; set; }
}

public class GoogleSheetRow
{
    public GoogleSheetRow() => Cells = new List<GoogleSheetCell>();

    public List<GoogleSheetCell> Cells { get; set; }
}

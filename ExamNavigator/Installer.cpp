//установщик для нашего приложения, всё это можно вынести в отдельный класс, требует обсуждения

#include <iostream>
#include <windows.h>
#include <fstream> //для работы с файлами
#include <direct.h>
#include <tchar.h>

using namespace std;

/*  Const   */

const int ROWS = 420;
const int COLOMN = 720;

const int cellSize = 75;
int rowTables = 2;
int colTables = 7;

const int shiftRows = 50;//Shift menu
const int shiftCol = 25;
static HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
static HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
static HBRUSH RedBrush = CreateSolidBrush(RGB(255, 0, 0));
static HBRUSH GreenBrush = CreateSolidBrush(RGB(0, 255, 0));

/*  Const   */


/*Переменные*/


int** tables{ new int* [rowTables] {} };

void initializationTables(int rowTables, int colTables) {
    for (int i = 0; i < rowTables; i++) {
        tables[i] = new int[colTables];
    }
}

int** reinitializationTables(int rowTables, int colTables) { //для изменения размеров массива для данных по лабам

    int** newTables{ new int* [rowTables] {} };
    for (int i = 0; i < rowTables; i++) {
        newTables[i] = new int[colTables];
    }
    for (int i = 0; i < rowTables; i++) {
        for (int j = 0; j < colTables; j++) {
            newTables[i][j] = tables[i][j];
        }
    }
    delete[] tables;
    return newTables;
}

HDC hdc;

struct Discipline { //в основном файле это можно вынести в конструктор класса, в установщике думаю хватит структуры(?) Всё больше склоняюсь к классу, для работы с динмассивом
    int ControlWorks = 2; //количество контрольных работ
    int Labs = 8; //количество лабораторных работ
    string WaysDataLabs = { "" }; //путь к файлу
    string WaysDataControls = { "" };

    int* MassiveLabs = new int[Labs];//массив хранящий данные по лабораторной работе
    int* MassiveControlWorks = new int[ControlWorks];//массив для оценок за контрольные работы
    int scores;//количество баллов
    bool FlagExamsOrTest = false; //флаг, если false, то зачет, если true, то Экзамен. Нужно для работы скриптов у ИИ 

    string name;
};

int CheckResource() {
    int check;
    fstream SettingFile("resourse/settings.txt", ios::in | ios::out);
    if (SettingFile.is_open()) {
        SettingFile >> check;
        SettingFile.close();
        return check;
    }
    else {
        SettingFile.close();
        return 0;
    }
}

void InstallerMain() { //доделать
    string way = { "resourse/" };//путь к папке с данными
    Discipline* MassiveDiscipline = new Discipline[3]; //инициализируем начальный маасив
    int countDiscipline = 0;
    int numberLabs, numberControl;

    int NumberDisciplinel = 1;
    //cin >> NumberDisciplinel;//считываем количество дисциплин
    //for (int i = 0; i < NumberDisciplinel; i++) {
    //    system("cls");

    //    cout << "Список дисциплин: " << endl;
    //    for (int i = 0; i < countDiscipline; i++) {
    //        cout << MassiveDiscipline[i].name << endl;
    //    }

    //    cout << "Введите название дисциплины (Английский язык!!!) или нажмите Esc для выхода:" << endl;
    //    string DisciplineName;
    //    cin >> DisciplineName;
    //    if (DisciplineName.empty()) {
    //        cout << "Пустой ввод, выход." << endl;
    //        break;
    //    }

    //    string path = way + DisciplineName;

    //    if (_mkdir((path).c_str()) == 0) {
    //        Discipline* newMassiveDiscipline = new Discipline[countDiscipline + 1];//увеличиваем длину массива

    //        for (int i = 0; i < countDiscipline; i++) {
    //            newMassiveDiscipline[i] = MassiveDiscipline[i]; //перезапись массива
    //        }
    //        delete[] MassiveDiscipline;

    //        MassiveDiscipline = newMassiveDiscipline;

    //        MassiveDiscipline[countDiscipline].WaysDataLabs = way + DisciplineName + "/" + DisciplineName + "Labs" + ".txt";
    //        MassiveDiscipline[countDiscipline].name = DisciplineName;
    //        //cout << "Введите количество лабораторных работ" << endl;
    //        cin >> numberLabs;//организовать считывание
    //        MassiveDiscipline[countDiscipline].Labs = numberLabs;

    //        //cout << "Введите количество контрольных работ" << endl;
    //        cin >> numberControl;
    //        MassiveDiscipline[countDiscipline].ControlWorks = numberControl;
    //        if (numberControl > 0) {
    //            MassiveDiscipline[countDiscipline].WaysDataControls = way + DisciplineName + "/" + DisciplineName + "Controls.txt";
    //        }

    //        fstream FileLabs(MassiveDiscipline[countDiscipline].WaysDataLabs, ios::in | ios::app);
    //        if (FileLabs.is_open()) {
    //            for (int i = 0; i < numberLabs; i++) {
    //                FileLabs << " X |";
    //            }
    //        }
    //        FileLabs.close();

    //        fstream FileControls(MassiveDiscipline[countDiscipline].WaysDataControls, ios::in | ios::app);
    //        if (FileControls.is_open()) {
    //            for (int i = 0; i < numberControl; i++) {
    //                FileControls << " X |";
    //            }
    //        }
    //        countDiscipline++;
    //        FileControls.close();

    //    }
    //    else {
    //        cerr << "Не удалось открыть папку с ресурсами" << endl;
    //    }

    //}
    ofstream SettingFiles("resourse/settings.txt", ios::out | ios::trunc);
    SettingFiles << 1 << endl;
    delete[] MassiveDiscipline;
}



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"Installer";

    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    // Create Window
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"InstallerExamNavigator",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        //size Window
        CW_USEDEFAULT, CW_USEDEFAULT, COLOMN, ROWS,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

void DrawTable(int** tables);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static int sX, sY;
    switch (uMsg) {
    case WM_SIZE:
        sX = LOWORD(lParam);
        sY = LOWORD(wParam);
        break; // Add break here

    case WM_CREATE:
        //int CheckSettings = CheckResource();//проверяем, вводились ли данные
        //if (CheckSettings == 0) { Installer(); }//если в файле 0, то 
        initializationTables(rowTables, colTables);
        InstallerMain();
        break; // Add break here

    case WM_PAINT: {
        PAINTSTRUCT ps;
        hdc = BeginPaint(hwnd, &ps);
        //DrawTable(tables);
        EndPaint(hwnd, &ps);
        break; // Add break here
    }

    case WM_LBUTTONDOWN: {
        int xPos = LOWORD(lParam);
        int yPos = HIWORD(lParam);

        int col = (xPos - shiftCol) / cellSize;//вычисляем позицию у щелчка мышкой
        int row = (yPos - shiftRows) / cellSize;

        // Validate row and col to prevent out-of-bounds access
        if (col >= 0 && col < COLOMN && row >= 0 && row < ROWS) {
            InvalidateRect(hwnd, NULL, TRUE); // redraw full Window
        }
        char buffer[256]; // Буфер для форматирования текста
        sprintf_s(buffer, 256, "Вы нажали на клетку %d %d", row, col); // Форматируем строку

        MessageBoxA(NULL, buffer, "Debug Values", MB_OK); // Отображаем MessageBox
        break; // Add Break here
    }

    case WM_RBUTTONDOWN: {
        int xPos = LOWORD(lParam);  // horizontal position of cursor
        int yPos = HIWORD(lParam);  // vertical position of cursor

        int col = (xPos) / cellSize;
        int row = (yPos) / cellSize;
        InvalidateRect(hwnd, NULL, TRUE);
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}


// *Приложение только включено* -> Введите название дисциплины -> 
// -> введите все данные по дисциплине (думаю, будет табличка 1 на 7 с ячейками куда будет вводить) -> Продолжить ввод новых дисциплин?
//И всплывает 2 кнопки Да/Нет. Если нажата "нет", соответственно выходим из установщика

void drawButton(int x, int y, HBRUSH brush, bool YesNo);

void DrawTable(int** tables) {

    for (int y = 0; y < rowTables; y++) {
        for (int x = 0; x < colTables; x++) {
            RECT cellRect;
            cellRect.left = x * cellSize + shiftCol;
            cellRect.right = (x + 1) * cellSize + shiftCol;
            cellRect.top = y * cellSize + shiftRows;
            cellRect.bottom = (y + 1) * cellSize + shiftRows;

            FillRect(hdc, &cellRect, whiteBrush);
            FrameRect(hdc, &cellRect, blackBrush);
        }
    }
}


void drawButton(int x, int y, HBRUSH brush, bool YesNo) {
    RECT button;
    button.left = x;
    button.right = x + cellSize;
    button.top = y;
    button.bottom = y + 40;
    FillRect(hdc, &button, brush);
    FrameRect(hdc, &button, CreateSolidBrush(RGB(0, 0, 0)));
    COLORREF textColor = RGB(255, 255, 255);
    SetBkMode(hdc, TRANSPARENT);//Прозрачность

    if (YesNo) {
        TCHAR numStr[5] = L"Yes";
        SetTextColor(hdc, textColor);
        TextOut(hdc, x + 15, y + 10, numStr, _tcslen(numStr));
    }
    else {
        TCHAR numStr[5] = L"No";
        SetTextColor(hdc, textColor);
        TextOut(hdc, x + 15, y + 10, numStr, _tcslen(numStr));
    }
}

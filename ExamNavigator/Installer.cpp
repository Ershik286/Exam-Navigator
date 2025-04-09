#include <iostream>
#include <windows.h>
#include <fstream>
#include <direct.h>
#include <tchar.h>
#include <string>
#include <vector>

using namespace std;

// Константы
const int ROWS = 600;
const int COLOMN = 800;
const int cellSize = 75;
const int shiftRows = 50;
const int shiftCol = 25;
const int inputFieldWidth = 200;
const int buttonWidth = 75;
const int buttonHeight = 40;
const int inputFieldHeight = 25; // Adjust for the height of the input field

// Цвета и кисти
HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
HBRUSH buttonBrush = CreateSolidBrush(RGB(200, 200, 200));
COLORREF textColor = RGB(0, 0, 0);

// Структура для хранения информации о дисциплине
struct Discipline {
    string name;
    int labs;
    int controlWorks;
    bool isExam;

    Discipline() : labs(0), controlWorks(0), isExam(false) {}
};

// Глобальные переменные
vector<Discipline> disciplines;
int currentInput = 0;
string currentDisciplineName;
string currentLabsStr;
string currentControlWorksStr;

// Определения ID для элементов управления (edit control, button)
#define IDC_EDIT_DISCIPLINE_NAME 1001
#define IDC_EDIT_LABS 1002
#define IDC_EDIT_CONTROL_WORKS 1003
#define IDC_BUTTON_EXAM 1004
#define IDC_BUTTON_CREDIT 1005
#define IDC_STATIC_TEXT 1006

// Функция для отрисовки текста
void DrawText(HDC hdc, int x, int y, const wstring& text) {
    TextOutW(hdc, x, y, text.c_str(), text.length());
}

// Функция для обработки ввода данных по дисциплине
void createDisciplineInputControls(HWND hwnd) {
    int xLabel = 50;
    int yLabel = 100;
    int xInput = xLabel + 150;
    int yInput = yLabel;

    // 1. Создаем статический текст (метки)
    CreateWindowW(L"STATIC", L"Название дисциплины:", WS_VISIBLE | WS_CHILD, xLabel, yLabel, 140, 20, hwnd, (HMENU)IDC_STATIC_TEXT, GetModuleHandle(NULL), NULL);
    yLabel += 30;
    CreateWindowW(L"STATIC", L"Количество лабораторных:", WS_VISIBLE | WS_CHILD, xLabel, yLabel, 140, 20, hwnd, (HMENU)IDC_STATIC_TEXT, GetModuleHandle(NULL), NULL);
    yLabel += 30;
    CreateWindowW(L"STATIC", L"Количество контрольных:", WS_VISIBLE | WS_CHILD, xLabel, yLabel, 140, 20, hwnd, (HMENU)IDC_STATIC_TEXT, GetModuleHandle(NULL), NULL);

    // 2. Создаем поля ввода (Edit Control)
    yLabel = 100;
    yInput = yLabel;
    HWND editName = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, xInput, yInput, inputFieldWidth, inputFieldHeight, hwnd, (HMENU)IDC_EDIT_DISCIPLINE_NAME, GetModuleHandle(NULL), NULL);
    yLabel += 30;
    yInput = yLabel;
    HWND editLabs = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, xInput, yInput, inputFieldWidth, inputFieldHeight, hwnd, (HMENU)IDC_EDIT_LABS, GetModuleHandle(NULL), NULL);
    yLabel += 30;
    yInput = yLabel;
    HWND editControlWorks = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, xInput, yInput, inputFieldWidth, inputFieldHeight, hwnd, (HMENU)IDC_EDIT_CONTROL_WORKS, GetModuleHandle(NULL), NULL);

    // 3. Создаем кнопки
    CreateWindowW(L"BUTTON", L"Экзамен", WS_VISIBLE | WS_CHILD, xInput + 250, 100, 80, 30, hwnd, (HMENU)IDC_BUTTON_EXAM, GetModuleHandle(NULL), NULL);
    CreateWindowW(L"BUTTON", L"Зачет", WS_VISIBLE | WS_CHILD, xInput + 250, 130, 80, 30, hwnd, (HMENU)IDC_BUTTON_CREDIT, GetModuleHandle(NULL), NULL);

}

// Функция для отображения информации о введенных дисциплинах
void showDisciplines(HDC hdc) {
    int yOffset = 250; // Starting vertical position for listing
    int xText = 50;
    DrawText(hdc, xText, yOffset, L"Введенные дисциплины:");
    yOffset += 30;

    for (const auto& discipline : disciplines) {
        wstring output = L"";
        output += wstring(discipline.name.begin(), discipline.name.end());
        output += L"  Labs: ";
        output += to_wstring(discipline.labs);
        output += L"  Controls: ";
        output += to_wstring(discipline.controlWorks);
        output += L"  ";
        output += discipline.isExam ? L"Экзамен" : L"Зачет";
        DrawText(hdc, xText, yOffset, output);
        yOffset += 20;
    }
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    HDC hdc;
    PAINTSTRUCT ps;
    static HWND hEditName, hEditLabs, hEditControlWorks; // Store edit control handles

    switch (uMsg) {
    case WM_CREATE:
        // Инициализация окна
        createDisciplineInputControls(hwnd);
        break;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Разобрать выбор пользователя:
        switch (wmId)
        {
        case IDC_BUTTON_EXAM:
        {
            // Получаем значения из Edit Control
            wchar_t nameBuffer[256];
            wchar_t labsBuffer[256];
            wchar_t controlWorksBuffer[256];

            HWND hEditName = GetDlgItem(hwnd, IDC_EDIT_DISCIPLINE_NAME);
            HWND hEditLabs = GetDlgItem(hwnd, IDC_EDIT_LABS);
            HWND hEditControlWorks = GetDlgItem(hwnd, IDC_EDIT_CONTROL_WORKS);

            GetWindowTextW(hEditName, nameBuffer, 256);
            GetWindowTextW(hEditLabs, labsBuffer, 256);
            GetWindowTextW(hEditControlWorks, controlWorksBuffer, 256);

            // Преобразуем wchar_t* в string
            wstring wstrName(nameBuffer);
            string strName(wstrName.begin(), wstrName.end());

            wstring wstrLabs(labsBuffer);
            string strLabs(wstrLabs.begin(), wstrLabs.end());

            wstring wstrControlWorks(controlWorksBuffer);
            string strControlWorks(wstrControlWorks.begin(), wstrControlWorks.end());


            // Создаем объект Discipline
            Discipline newDiscipline;
            newDiscipline.name = strName;

            try {
                newDiscipline.labs = stoi(strLabs);
                newDiscipline.controlWorks = stoi(strControlWorks);
            }
            catch (const std::invalid_argument& ia) {
                MessageBoxW(hwnd, L"Некорректный ввод чисел.", L"Ошибка", MB_OK | MB_ICONERROR);
                break; // Выходим из case, не добавляем дисциплину
            }

            newDiscipline.isExam = true;
            disciplines.push_back(newDiscipline);

            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }

        case IDC_BUTTON_CREDIT:
        {
            // Получаем значения из Edit Control
            wchar_t nameBuffer[256];
            wchar_t labsBuffer[256];
            wchar_t controlWorksBuffer[256];

            HWND hEditName = GetDlgItem(hwnd, IDC_EDIT_DISCIPLINE_NAME);
            HWND hEditLabs = GetDlgItem(hwnd, IDC_EDIT_LABS);
            HWND hEditControlWorks = GetDlgItem(hwnd, IDC_EDIT_CONTROL_WORKS);

            GetWindowTextW(hEditName, nameBuffer, 256);
            GetWindowTextW(hEditLabs, labsBuffer, 256);
            GetWindowTextW(hEditControlWorks, controlWorksBuffer, 256);

            // Преобразуем wchar_t* в string
            wstring wstrName(nameBuffer);
            string strName(wstrName.begin(), wstrName.end());

            wstring wstrLabs(labsBuffer);
            string strLabs(wstrLabs.begin(), wstrLabs.end());

            wstring wstrControlWorks(controlWorksBuffer);
            string strControlWorks(wstrControlWorks.begin(), wstrControlWorks.end());


            // Создаем объект Discipline
            Discipline newDiscipline;
            newDiscipline.name = strName;

            try {
                newDiscipline.labs = stoi(strLabs);
                newDiscipline.controlWorks = stoi(strControlWorks);
            }
            catch (const std::invalid_argument& ia) {
                MessageBoxW(hwnd, L"Некорректный ввод чисел.", L"Ошибка", MB_OK | MB_ICONERROR);
                break; // Выходим из case, не добавляем дисциплину
            }

            newDiscipline.isExam = false;
            disciplines.push_back(newDiscipline);

            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }
        }
        break;
    }

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, whiteBrush);

        showDisciplines(hdc);

        EndPaint(hwnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"InstallerClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = whiteBrush;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Дисциплины",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, COLOMN, ROWS,
        NULL,
        NULL,
        hInstance,
        NULL
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

//установщик для нашего приложения, всё это можно вынести в отдельный класс, требует обсуждения

#include <iostream>
#include <windows.h>
#include <fstream> //для работы с файлами
#include <direct.h>

using namespace std;

struct Discipline { //в основном файле это можно вынести в конструктор класса, в установщике думаю хватит структуры(?) Всё больше склоняюсь к классу, для работы с динмассивом
	int ControlWorks = 2; //количество контрольных работ
	int Labs = 8; //количество лабораторных работ
	string WaysDataLabs = { "" }; //путь к файлу
	string WaysDataControls = { "" };

	int* MassiveLabs = new int [Labs];//массив хранящий данные по лабораторной работе
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

void Installer() {
	string way = { "resourse/" };//путь к папке с данными
    Discipline* MassiveDiscipline = new Discipline[3]; // Initial size
    int countDiscipline = 0;
    int numberLabs, numberControl;

    int NumberDisciplinel = 1;
    cin >> NumberDisciplinel;
    for (int i = 0; i < NumberDisciplinel; i++) {
        system("cls");

        cout << "Список дисциплин: " << endl;
        for (int i = 0; i < countDiscipline; i++){
            cout << MassiveDiscipline[i].name << endl;
        }

        cout << "Введите название дисциплины (Английский язык!!!) или нажмите Esc для выхода:" << endl;
        string DisciplineName;
        cin >> DisciplineName;
        if (DisciplineName.empty()) {
            cout << "Пустой ввод, выход." << endl;
            break;
        }

        string path = way + DisciplineName;

        if (_mkdir((path).c_str()) == 0) {
            Discipline* newMassiveDiscipline = new Discipline[countDiscipline + 1];

            for (int i = 0; i < countDiscipline; i++) {
                newMassiveDiscipline[i] = MassiveDiscipline[i];
            }
            delete[] MassiveDiscipline;

            MassiveDiscipline = newMassiveDiscipline;

            MassiveDiscipline[countDiscipline].WaysDataLabs = way + DisciplineName + "/" + DisciplineName + "Labs" + ".txt";
            MassiveDiscipline[countDiscipline].name = DisciplineName;
            cout << "Введите количество лабораторных работ" << endl;
            cin >> numberLabs;
            MassiveDiscipline[countDiscipline].Labs = numberLabs;

            cout << "Введите количество контрольных работ" << endl;
            cin >> numberControl;
            MassiveDiscipline[countDiscipline].ControlWorks = numberControl;
            if (numberControl > 0) {
                MassiveDiscipline[countDiscipline].WaysDataControls = way + DisciplineName + "/" + DisciplineName + "Controls.txt";
            }

            fstream FileLabs(MassiveDiscipline[countDiscipline].WaysDataLabs, ios::in | ios::app);
            if (FileLabs.is_open()) {
                for (int i = 0; i < numberLabs; i++) {
                    FileLabs << " X |";
                }
            }
            FileLabs.close();

            fstream FileControls(MassiveDiscipline[countDiscipline].WaysDataControls, ios::in | ios::app);
            if (FileControls.is_open()) {
                for (int i = 0; i < numberControl; i++) {
                    FileControls << " X |";
                }
            }
            countDiscipline++;
            FileControls.close();

        }
        else {
            cerr << "Не удалось открыть папку с ресурсами" << endl;
        }

    }
    ofstream SettingFiles("resourse/settings.txt", ios::out | ios::trunc);
    SettingFiles << 1 << endl;
	delete[] MassiveDiscipline;
}

int main() {//пока примерный консольный вид работы
	int CheckSettings = CheckResource();//проверяем, вводились ли данные
	if (CheckSettings == 0) { Installer(); }//если в файле 0, то устанавливаем базы данных дисциплин
    else { cout << "ы"; }
}

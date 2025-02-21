//установщик для нашего приложения, всё это можно вынести в отдельный класс, требует обсуждения

#include <iostream>
#include <Windows.h>
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

void Installer() {
	string way = { "resourse/" };//путь к папке с данными
	int numberOfDisciplines = 3; //для начала возьмём 3 дисциплины, я думаю 
	Discipline* MassiveDiscipline = new Discipline[numberOfDisciplines];//динамический массив для всех дисциплин. 
	//Если будет больше, то будем пересоздавать массив, если разработкой буду заниматься не Я(Никита), 
	// то аккуратней, не забывайте удалять и чистить память, чтобы не было утечек.
	int countDiscipline = 0;//счетчик для возможного увеличения длины массива
	int numberLabs;
	int numberControl;
	while (true) {
		system("cls");
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) { // Check highest-order bit
			break; // Exit immediately if Escape is pressed
		}
		cout << "Для завершения работы программы нажмите Esc" << endl;
		if (countDiscipline > 3) {//превысило начальное количество
			delete[] MassiveDiscipline;//удаляем массив
			Discipline* MassiveDiscipline = new Discipline[countDiscipline];
		}
		cout << "Введите название дисциплины (Английский язык!!!)" << endl;
		string DisciplineName;
		cin >> DisciplineName;
		string path = way + DisciplineName;
		if (_mkdir((path).c_str()) == 0) {//удалось ли создать папку
			MassiveDiscipline[countDiscipline].WaysDataLabs = way + DisciplineName + "/" + DisciplineName + "Labs" + ".txt";//записали в структуру путь к дисциплине
			MassiveDiscipline[countDiscipline].name = DisciplineName;
			cout << "Введите количество лабораторных работ" << endl;
			cin >> numberLabs;//возможно, надо будет использовать scanf из си, дабы проверять ввод. Но я не уверен будем ли мы заходить в такие дебри
			MassiveDiscipline[countDiscipline].Labs = numberLabs;

			cout << "Введите количество контрольных работ" << endl;//? под вопросом, пользователь может не знать количество контрольных работ по дисциплине
			cin >> numberControl;
			MassiveDiscipline[countDiscipline].ControlWorks = numberControl;
			if (numberControl > 0) {
				MassiveDiscipline[countDiscipline].WaysDataControls = way + DisciplineName + "/" + DisciplineName + "Controls.txt";
			}

			if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) { // нажато Esc
				break; // Exit immediately if Escape is pressed
			}
			fstream FileLabs(MassiveDiscipline[countDiscipline].WaysDataLabs, ios::in | ios::app);//создаем файл с лабой и открываем его
			if (FileLabs.is_open()) {
				for (int i = 0; i < numberLabs; i++) {
					FileLabs << " X |";//крестик - не сдана
				}
			}
			FileLabs.close();
			fstream FileControls(MassiveDiscipline[countDiscipline].WaysDataControls, ios::in | ios::app);//создаем файл с контрольными работами и открываем его
			if (FileControls.is_open()) {
				for (int i = 0; i < numberControl; i++) {
					FileControls << " X |";
				}
			}
			countDiscipline++;
		}
	}
}

int main() {//пока примерный консольный вид работы
	Installer();
}
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

struct Student {
    string name;
    int score;
};

// Функции сравнения
bool compareByName(int i, int j, const vector<Student>& students) {
    return students[i].name < students[j].name;
}

bool compareByScore(int i, int j, const vector<Student>& students) {
    return students[i].score > students[j].score;
}

// Рекурсивная функция бинарного поиска
int binarySearch(const vector<int>& indices, const string& key, const vector<Student>& students, int left, int right, bool byName) {
    if (left > right) return -1;

    int mid = left + (right - left) / 2;
    int index = indices[mid];

    if ((byName && students[index].name == key) || (!byName && to_string(students[index].score) == key)) {
        return index;
    } else if ((byName && students[index].name < key) || (!byName && students[index].score < stoi(key))) {
        return binarySearch(indices, key, students, mid + 1, right, byName);
    } else {
        return binarySearch(indices, key, students, left, mid - 1, byName);
    }
}

// Функция обновления индексов
void updateIndices(vector<Student>& students, vector<int>& nameIndices, vector<int>& scoreIndices) {
    sort(nameIndices.begin(), nameIndices.end(), [&](int i, int j){ return compareByName(i, j, students); });
    sort(scoreIndices.begin(), scoreIndices.end(), [&](int i, int j){ return compareByScore(i, j, students); });
}

// Функция удаления записи по имени
void removeStudentByName(string nameToRemove, vector<Student>& students, vector<int>& nameIndices, vector<int>& scoreIndices) {
    auto it = find_if(students.begin(), students.end(), [&](const Student& s) { return s.name == nameToRemove; });

    if (it != students.end()) {
        int indexToRemove = distance(students.begin(), it);
        students.erase(it);

        auto itName = find(nameIndices.begin(), nameIndices.end(), indexToRemove);
        if (itName != nameIndices.end()) {
            nameIndices.erase(itName);
        }
        auto itScore = find(scoreIndices.begin(), scoreIndices.end(), indexToRemove);
        if (itScore != scoreIndices.end()) {
            scoreIndices.erase(itScore);
        }

        for (size_t i = 0; i < nameIndices.size(); ++i) {
            if (nameIndices[i] > indexToRemove) {
                nameIndices[i]--;
            }
        }
        for (size_t i = 0; i < scoreIndices.size(); ++i) {
                        if (scoreIndices[i] > indexToRemove) {
                scoreIndices[i]--;
            }
        }
    } else {
        cout << "Студент '" << nameToRemove << "' не найден.\n";
    }
}

// Функция вывода отсортированного списка
void printSortedList(const vector<Student>& students, const vector<int>& indices, const string& sortType) {
    cout << "\nОтсортированный список по " << sortType << ":\n";
    for (int i : indices) {
        cout << students[i].name << " - " << students[i].score << endl;
    }
}

int main() {
    int n;
    cout << "Введите количество студентов: ";
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<Student> students(n);
    for (int i = 0; i < n; ++i) {
        cout << "Введите имя студента " << i + 1 << ": ";
        getline(cin, students[i].name);
        cout << "Введите баллы студента " << i + 1 << ": ";
        cin >> students[i].score;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    vector<int> nameIndices(n);
    vector<int> scoreIndices(n);
    for (int i = 0; i < n; ++i) {
        nameIndices[i] = i;
        scoreIndices[i] = i;
    }

    updateIndices(students, nameIndices, scoreIndices);

    printSortedList(students, nameIndices, "имени");
    printSortedList(students, scoreIndices, "баллам");


    string input;
    while (true) {
        int choice;
        cout << "\nВыберите действие:\n";
        cout << "1. Редактировать данные\n";
        cout << "2. Удалить данные\n";
        cout << "3. Найти студента по имени\n";
        cout << "4. Найти студента по баллам\n";
        cout << "Введите ваш выбор (1, 2, 3, 4 или 'end' для выхода): ";
        getline(cin, input);

        if (input == "end") {
            break;
        }

        try {
            choice = stoi(input);
            switch (choice) {
                case 1: {
                    int editIndex;
                    cout << "\nВведите индекс студента для редактирования (0-" << students.size() - 1 << "): ";
                    cin >> editIndex;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (editIndex >= 0 && editIndex < students.size()) {
                        cout << "Текущее имя: " << students[editIndex].name << ", текущий балл: " << students[editIndex].score << endl;
                        cout << "Введите новое имя: ";
                        getline(cin, students[editIndex].name);
                        cout << "Введите новый балл: ";
                        cin >> students[editIndex].score;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        updateIndices(students, nameIndices, scoreIndices);
                        printSortedList(students, nameIndices, "имени");
                        printSortedList(students, scoreIndices, "баллам");
                    } else {
                        cout << "Неверный индекс студента." << endl;
                    }
                    break;
                }
                case 2: {
                    string nameToRemove;
                    cout << "\nВведите имя студента для удаления: ";
                    getline(cin, nameToRemove);
                    removeStudentByName(nameToRemove, students, nameIndices, scoreIndices);
                    printSortedList(students, nameIndices, "имени");
                    printSortedList(students, scoreIndices, "баллам");
                    break;
                }
                case 3: {
                    string searchName;
                    cout << "\nВведите имя студента для поиска: ";
                    getline(cin, searchName);
                    int foundIndex = binarySearch(nameIndices, searchName, students, 0, nameIndices.
                    size() - 1, true);
                    if (foundIndex != -1) {
                        cout << "Студент найден: " << students[foundIndex].name << " - " << students[foundIndex].score << endl;
                    } else {
                        cout << "Студент с именем '" << searchName << "' не найден." << endl;
                    }
                    break;
                }
                case 4: {
                    string searchScore;
                    cout << "\nВведите баллы студента для поиска: ";
                    getline(cin, searchScore);
                    int foundIndex = binarySearch(scoreIndices, searchScore, students, 0, scoreIndices.size() - 1, false);
                    if (foundIndex != -1) {
                        cout << "Студент найден: " << students[foundIndex].name << " - " << students[foundIndex].score << endl;
                    } else {
                        cout << "Студент с баллами '" << searchScore << "' не найден." << endl;
                    }
                    break;
                }
                default:
                    cout << "Неверный выбор.\n";
            }
        } catch (const invalid_argument& e) {
            cout << "Неверный ввод.  Пожалуйста, введите число от 1 до 4 или 'end' для выхода.\n";
        } catch (const out_of_range& e) {
            cout << "Неверный ввод.\n";
        }
    }

    cout << "Программа завершена.\n";
    return 0;
}


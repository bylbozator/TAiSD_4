#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <windows.h>
#include <algorithm>
using namespace std;

struct MonthlyIncome {
    int year;
    int month;
    string department;
    double income;


};


struct DepartmentIncome {
    string department;
    double totalIncome;
    int numEntries;
  
    int longestPeriod; // наиболее длинный период с доходом ниже среднего по всей фирме
};

void viewDatabase(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }
    else {
        cout << "Невозможно открыть базу данных." << endl;
    }
}

bool isDataValid(const string& year, const string& month, const string& department, int income) {
    // Проверяем, что год состоит из 4 цифр
    if (year.length() != 4 || !all_of(year.begin(), year.end(), ::isdigit)) {
        return false;
    }

    // Проверяем, что месяц состоит из 2 цифр
    if (month.length() != 2 || !all_of(month.begin(), month.end(), ::isdigit)) {
        return false;
    }

    // Проверяем, что отдел не пустой
    if (department.empty()) {
        return false;
    }

    // Проверяем, что доход положительный
    if (income <= 0) {
        return false;
    }

    return true;
}



void calculateAverageIncome(const string& filename) {
    ifstream file(filename);
    viewDatabase(filename);
        string line;
        vector<DepartmentIncome> departmentIncomes;

        while (getline(file, line)) {
            if (line.find("Год") != string::npos) {// Пропустить строку заголовка
                continue;
            }

            stringstream ss(line);
            int year, month;
            string department;
            double income;

            ss >> year >> month >> department >> income;

            // Проверьте, существует ли отдел уже в разделе Доходы отдела
            bool found = false;
            for (auto& entry : departmentIncomes) {
                if (entry.department == department) {
                    entry.totalIncome += income;
                    entry.numEntries++;
                    found = true;
                    break;
                }
            }

            // Если отдел не существует, создайте новую запись
            if (!found) {
                DepartmentIncome newEntry = { department, income, 1 };
                departmentIncomes.push_back(newEntry);
            }
        }

        file.close();

        cout << "Средний доход на отдел за пять лет:"<<endl;
        for (const auto& entry : departmentIncomes) {
            double averageIncome = entry.totalIncome / entry.numEntries;
            cout << "Отдел: " << entry.department << ",Средний доход: " << averageIncome << endl;
        }
    
    
}



void editDatabase(const string& filename) {
    // Шаг 1: Открыть файл для чтения и записи
    fstream file(filename);
  

    // Шаг 2: Прочитать все данные из файла и сохранить их в векторе
    vector<MonthlyIncome> data;
    MonthlyIncome entry;
    while (file >> entry.year >> entry.month >> entry.department >> entry.income) {
        data.push_back(entry);
    }

    // Шаг 3: Показать текущие данные пользователю
    cout << "Текущие данные:" << endl;
    viewDatabase(filename);
    // Шаг 4: Запросить новые значения от пользователя
    cout << "Введите год, месяц и отдел, которые нужно корректировать (через пробел): ";
    int newYear;
    int newmonth;
    string newdepartment;
    cin >> newYear >> newmonth>> newdepartment;

    // Шаг 5: Изменить значения в векторе данных
    for (auto& entry : data) {
        if (entry.year == newYear && entry.month == newmonth && entry.department==newdepartment) {
            cout << "Текущее значение дохода: " << entry.income << endl;
            cout << "Введите новое значение дохода: ";
            cin >> entry.income;
        }
    }

    // Шаг 6: Открыть файл для перезаписи
    file.close();
    file.open(filename, ios::out | ios::trunc); // открываем файл для перезаписи

    // Шаг 7: Записать измененные данные в файл
    for (const auto& entry : data) {
        file << entry.year << " " << entry.month << " " << entry.department << " " << entry.income << endl;
    }

    // Шаг 8: Закрыть файл
    file.close();

    cout << "Данные успешно обновлены." << endl;
}


void deleteFromDatabase(const string& filename) {
    // Шаг 1: Открыть файл для чтения и записи
    fstream file(filename);

        // Шаг 2: Прочитать все данные из файла и сохранить их в векторе
        vector<MonthlyIncome> data;
    MonthlyIncome entry;
    while (file >> entry.year >> entry.month >> entry.department >> entry.income) {
        data.push_back(entry);
    }

    // Шаг 3: Показать текущие данные пользователю
    cout << "Текущие данные:" << endl;
    viewDatabase(filename);

    // Шаг 4: Запросить значения для удаления от пользователя
    cout << "Введите год, месяц и отдел, которые нужно удалить (через пробел): ";
    int deleteYear;
    int deleteMonth;
    string deleteDepartment;
    cin >> deleteYear >> deleteMonth >> deleteDepartment;

    // Шаг 5: Удалить записи из вектора данных
    data.erase(remove_if(data.begin(), data.end(), [&](const MonthlyIncome& entry) {
        return entry.year == deleteYear && entry.month == deleteMonth && entry.department == deleteDepartment;
        }), data.end());

    // Шаг 6: Открыть файл для перезаписи
    file.close();
    file.open(filename, ios::out | ios::trunc); // открываем файл для перезаписи

    // Шаг 7: Записать измененные данные в файл
    for (const auto& entry : data) {
        file << entry.year << " " << entry.month << " " << entry.department << " " << entry.income << endl;
    }

    // Шаг 8: Закрыть файл
    file.close();

    cout << "Данные успешно удалены." << endl;
}



void createDatabase(const string& filename) {
    ofstream file(filename, ios::app); // открываем файл для добавления данных
    viewDatabase(filename);
    string year, month, department;
    int income;
    string exitCommand = "exit";

    while (true) {
        cout << "Введите год, месяц, отдел и доход (через пробел) или введите 'exit' для выхода: ";
        cin >> year;

        // проверяем, введено ли слово "exit"
        if (year == exitCommand) {
            break;
        }

        cin >> month >> department >> income;
        if (isDataValid(year, month, department, income)) {
            file << year << " " << month << " " << department << " " << income << endl;
        }
        else {
            cout << "Введены некорректные данные. Попробуйте снова." << endl;
        }
    }

    file.close();
    cout << "Данные успешно добавлены в базу." << endl;
}



void calculateAverageIncomeAndLongestPeriod(const string& filename) {
    ifstream file(filename);
  

    string line;
    vector<DepartmentIncome> departmentIncomes;

    while (getline(file, line)) {
        if (line.find("Год") != string::npos) {
            continue;
        }

        stringstream ss(line);
        int year, month;
        string department;
        double income;

        ss >> year >> month >> department >> income;

        bool found = false;
        for (auto& entry : departmentIncomes) {
            if (entry.department == department) {
                entry.totalIncome += income;
                entry.numEntries++;

                // Проверяем, если текущий период длиннее предыдущего наиболее длинного периода,
                // обновляем наиболее длинный период для этого отдела
                if (entry.numEntries > entry.longestPeriod) {
                    entry.longestPeriod = entry.numEntries;
                }

                found = true;
                break;
            }
        }

        if (!found) {
            DepartmentIncome newEntry = { department, income, 1, 1 };
            departmentIncomes.push_back(newEntry);
        }
    }

    file.close();

    cout << "Средний доход на отдел за пять лет:" << endl;
    for (const auto& entry : departmentIncomes) {
        double averageIncome = entry.totalIncome / entry.numEntries;
        cout << "Отдел: " << entry.department << ", Средний доход: " << averageIncome << endl;
        cout << "Наиболее длинный период с доходом ниже среднего по всей фирме: " << entry.longestPeriod << endl;
    }
}






int main() {
    setlocale(0, "");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    string filename = "monthly_income_data.txt";
    ifstream inputFile("monthly_income_data.txt");
    int choice;

    do {
      
        cout << "Меню:" << endl;
        cout << "1. Создать или добавить базу данных" << endl;
        cout << "2. Просмотреть базу данных" << endl;
        cout << "3. Удалить запись" << endl;
        cout << "4. Исправить базу данных" << endl;
        cout << "5. Вычислить средний доход по отделу" << endl;
        cout << "И найти самый длинный период низкого дохода" << endl;
        cout << "6. Выход" << endl;
        cout << "Введите ваш выбор: ";
        cin >> choice;
        switch (choice) {
        case 1:
            createDatabase(filename);
            break;
        case 2:
            viewDatabase(filename);
            break;
        case 3: {
            deleteFromDatabase(filename);
        }
              break;
        case 4: {
            editDatabase(filename);
        }
              break;
        
        case 5: {
            calculateAverageIncomeAndLongestPeriod(filename);
        }  
            break;
        
        case 6:
        {
            cout << "Завершение программы." << endl;
            return 0;
        }
        default:
        {
            cout << "Неверный выбор. Пожалуйста, попробуйте снова." << endl;
        }
        }
    } 
    while (choice != 6);

    return 0;
}
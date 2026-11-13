#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <windows.h>
#include <vector>
#include <array>
#include <list>
#include <algorithm>

using namespace std;

// ==================== КОНСТАНТЫ ФАЙЛОВ ====================
const string DATA_FILE = "sport_data.txt";
const string REPORT_FILE = "sport_report.txt";

// ==================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ====================

static string padR(const string& s, size_t w) {
    if (s.length() >= w) return s;
    return s + string(w - s.length(), ' ');
}

// ==================== НАСТРОЙКИ ТАБЛИЦЫ ====================

namespace T {
    const int NUM = 4;
    const int NAME = 22;
    const int AGE = 8;
    const int COUNTRY = 15;
    const int SPORT = 18;
    const int EXP = 12;
    const int MEDALS = 10;
    const int SEP = 2;

    const int W_MAIN = NUM + NAME + AGE + COUNTRY + SPORT + EXP + MEDALS + 6 * SEP;
}

// ==================== ФУНКЦИИ ВВОДА ====================

int askInt(const string& prompt) {
    string input;
    while (true) {
        cout << "  " << prompt;
        getline(cin, input);
        try {
            size_t pos;
            int val = stoi(input, &pos);
            if (pos == input.length()) return val;
        }
        catch (...) {}
        cout << "  [Ошибка] Введите целое число!\n";
    }
}

int askPositiveInt(const string& prompt) {
    while (true) {
        int val = askInt(prompt);
        if (val > 0) return val;
        cout << "  [Ошибка] Значение должно быть больше нуля!\n";
    }
}

string askNonEmptyString(const string& prompt) {
    string s;
    while (true) {
        cout << "  " << prompt;
        getline(cin, s);
        if (!s.empty()) return s;
        cout << "  [Ошибка] Строка не может быть пустой!\n";
    }
}

bool askBool(const string& prompt) {
    string input;
    while (true) {
        cout << "  " << prompt << " (1-да, 0-нет): ";
        getline(cin, input);
        if (input == "1") return true;
        if (input == "0") return false;
        cout << "  [Ошибка] Введите 1 или 0.\n";
    }
}

// ==================== КЛАССЫ ====================

class Person {
protected:
    string name;
    int age;
    string country;
    string sport;
    int experience;

public:
    Person(const string& n, int a, const string& c, const string& s, int exp)
        : name(n), age(a), country(c), sport(s), experience(exp) {
    }
    virtual ~Person() {}

    string getName() const { return name; }
    int getAge() const { return age; }
    string getCountry() const { return country; }
    string getSport() const { return sport; }
    int getExperience() const { return experience; }

    void setName(const string& n) { name = n; }
    void setAge(int a) { age = a; }
    void setCountry(const string& c) { country = c; }
    void setSport(const string& s) { sport = s; }
    void setExperience(int exp) { experience = exp; }

    virtual int getType() const = 0;
    virtual string getMedalsStr() const = 0;

    virtual void printRow(ostream& os, int num) const {
        os << "  "
            << padR(to_string(num), T::NUM) << string(T::SEP, ' ')
            << padR(name, T::NAME) << string(T::SEP, ' ')
            << padR(to_string(age), T::AGE) << string(T::SEP, ' ')
            << padR(country, T::COUNTRY) << string(T::SEP, ' ')
            << padR(sport, T::SPORT) << string(T::SEP, ' ')
            << padR(to_string(experience), T::EXP) << string(T::SEP, ' ')
            << padR(getMedalsStr(), T::MEDALS) << "\n";
    }
};

class Athlete : public Person {
    int medals;
public:
    Athlete(const string& n, int a, const string& c, const string& s, int exp, int m)
        : Person(n, a, c, s, exp), medals(m) {
    }

    int getType() const override { return 1; }
    string getMedalsStr() const override { return to_string(medals); }

    int getMedals() const { return medals; }
    void setMedals(int m) { medals = m; }
};

class Coach : public Person {
public:
    Coach(const string& n, int a, const string& c, const string& s, int exp)
        : Person(n, a, c, s, exp) {
    }

    int getType() const override { return 2; }
    string getMedalsStr() const override { return "-"; }
};

class Event {
private:
    string eventName;
    string date;
    vector<Athlete*> athletes;
    vector<Coach*> coaches;

public:
    Event() : eventName(""), date("") {}
    Event(const string& name, const string& d) : eventName(name), date(d) {}

    ~Event() { clear(); }

    Event(const Event&) = delete;
    Event& operator=(const Event&) = delete;

    void clear() {
        for (Athlete* a : athletes) delete a;
        for (Coach* c : coaches) delete c;
        athletes.clear();
        coaches.clear();
    }

    string getEventName() const { return eventName; }
    string getDate() const { return date; }
    void setEventName(const string& n) { eventName = n; }
    void setDate(const string& d) { date = d; }

    size_t athleteCount() const { return athletes.size(); }
    size_t coachCount() const { return coaches.size(); }
    Athlete* getAthlete(size_t i) const { return athletes[i]; }
    Coach* getCoach(size_t i) const { return coaches[i]; }

    void addAthlete(Athlete* a) { athletes.push_back(a); }
    void addCoach(Coach* c) { coaches.push_back(c); }

    void removeAthlete(size_t index) {
        if (index >= athletes.size()) return;
        delete athletes[index];
        athletes.erase(athletes.begin() + index);
    }

    void removeCoach(size_t index) {
        if (index >= coaches.size()) return;
        delete coaches[index];
        coaches.erase(coaches.begin() + index);
    }

    void setAthlete(size_t index, Athlete* a) {
        if (index >= athletes.size()) return;
        delete athletes[index];
        athletes[index] = a;
    }

    void setCoach(size_t index, Coach* c) {
        if (index >= coaches.size()) return;
        delete coaches[index];
        coaches[index] = c;
    }

    void sortAthletes() {
        sort(athletes.begin(), athletes.end(),
            [](Athlete* a, Athlete* b) { return a->getExperience() > b->getExperience(); });
    }

    void sortCoaches() {
        sort(coaches.begin(), coaches.end(),
            [](Coach* a, Coach* b) { return a->getExperience() > b->getExperience(); });
    }
};

class SportsCommittee {
private:
    string committeeName;
    array<Event, 3> events;
    list<string> filterResults;

public:
    SportsCommittee() : committeeName("Городской спорткомитет") {
        events[0].setEventName("Кубок города");
        events[0].setDate("01.12.2026");
        events[1].setEventName("Чемпионат области");
        events[1].setDate("15.12.2026");
        events[2].setEventName("Первенство республики");
        events[2].setDate("25.12.2026");
    }

    Event& getEvent(int i) { return events[i]; }
    const array<Event, 3>& getEvents() const { return events; }

    void clear() {
        for (size_t i = 0; i < events.size(); ++i) {
            events[i].clear();
        }
    }

    void filterBySport(const string& sport) {
        filterResults.clear();
        for (size_t e = 0; e < events.size(); ++e) {
            for (size_t i = 0; i < events[e].athleteCount(); ++i) {
                Athlete* a = events[e].getAthlete(i);
                if (a->getSport() == sport) {
                    filterResults.push_back(events[e].getEventName() + " | " + a->getName() + " | Спортсмен");
                }
            }
            for (size_t i = 0; i < events[e].coachCount(); ++i) {
                Coach* c = events[e].getCoach(i);
                if (c->getSport() == sport) {
                    filterResults.push_back(events[e].getEventName() + " | " + c->getName() + " | Тренер");
                }
            }
        }
    }

    list<string>& getFilterResults() { return filterResults; }
};

// ==================== КРАСИВЫЙ ВЫВОД ====================

void printTableHeader(ostream& os) {
    os << "  "
        << padR("№", T::NUM) << string(T::SEP, ' ')
        << padR("ФИО", T::NAME) << string(T::SEP, ' ')
        << padR("Возраст", T::AGE) << string(T::SEP, ' ')
        << padR("Страна", T::COUNTRY) << string(T::SEP, ' ')
        << padR("Вид спорта", T::SPORT) << string(T::SEP, ' ')
        << padR("Опыт (лет)", T::EXP) << string(T::SEP, ' ')
        << padR("Медали", T::MEDALS) << "\n";
    os << "  " << string(T::W_MAIN, '-') << "\n";
}

void printEventTable(ostream& os, const Event& e) {
    os << "\n  " << string(T::W_MAIN, '=') << "\n";
    os << "    СОРЕВНОВАНИЕ: " << e.getEventName() << " (" << e.getDate() << ")\n";
    os << "  " << string(T::W_MAIN, '=') << "\n";

    os << "\n  -- Участники (Спортсмены) --\n";
    printTableHeader(os);
    if (e.athleteCount() == 0) os << "  (пусто)\n";
    else for (size_t i = 0; i < e.athleteCount(); ++i) e.getAthlete(i)->printRow(os, static_cast<int>(i + 1));

    os << "\n  -- Тренеры --\n";
    printTableHeader(os);
    if (e.coachCount() == 0) os << "  (пусто)\n";
    else for (size_t i = 0; i < e.coachCount(); ++i) e.getCoach(i)->printRow(os, static_cast<int>(i + 1));
}

void printAll(ostream& os, const SportsCommittee& committee) {
    os << "\n  " << string(T::W_MAIN, '=') << "\n";
    os << "    ВЕДОМОСТЬ СПОРТИВНЫХ МЕРОПРИЯТИЙ\n";
    os << "  " << string(T::W_MAIN, '=') << "\n";

    for (size_t i = 0; i < committee.getEvents().size(); ++i) {
        printEventTable(os, committee.getEvents()[i]);
    }

    os << "\n  (Отчет также сохранен в файле " << REPORT_FILE << ")\n";
}

// ==================== ФАЙЛОВЫЙ ВВОД/ВЫВОД ====================

void saveAll(const SportsCommittee& committee) {
    ofstream f(DATA_FILE);
    if (f.is_open()) {
        for (size_t e = 0; e < committee.getEvents().size(); ++e) {
            const Event& ev = committee.getEvents()[e];
            for (size_t i = 0; i < ev.athleteCount(); ++i) {
                Athlete* a = ev.getAthlete(i);
                f << "A|" << a->getName() << "|" << a->getAge() << "|" << a->getCountry()
                    << "|" << a->getSport() << "|" << a->getExperience() << "|" << a->getMedals() << "\n";
            }
            for (size_t i = 0; i < ev.coachCount(); ++i) {
                Coach* c = ev.getCoach(i);
                f << "C|" << c->getName() << "|" << c->getAge() << "|" << c->getCountry()
                    << "|" << c->getSport() << "|" << c->getExperience() << "\n";
            }
        }
        f.close();
    }

    ofstream frep(REPORT_FILE);
    if (frep.is_open()) {
        printAll(frep, committee);
        frep.close();
    }
}

bool loadAll(SportsCommittee& committee) {
    ifstream f(DATA_FILE);
    if (!f.is_open()) return false;

    string line;
    int evIdx = 0;
    bool hasData = false;
    while (getline(f, line)) {
        if (line.empty()) continue;
        hasData = true;
        stringstream ss(line);
        string type, name, country, sport;
        int age, exp, medals;

        getline(ss, type, '|');
        getline(ss, name, '|');
        string tempAge; getline(ss, tempAge, '|'); age = stoi(tempAge);
        getline(ss, country, '|');
        getline(ss, sport, '|');
        string tempExp; getline(ss, tempExp, '|'); exp = stoi(tempExp);

        if (type == "A") {
            string tempMed; getline(ss, tempMed); medals = stoi(tempMed);
            committee.getEvent(evIdx % 3).addAthlete(new Athlete(name, age, country, sport, exp, medals));
        }
        else if (type == "C") {
            committee.getEvent(evIdx % 3).addCoach(new Coach(name, age, country, sport, exp));
        }
        evIdx++;
    }
    return hasData;
}

void initData(SportsCommittee& committee) {
    // === Кубок города (01.12.2026) ===
    committee.getEvent(0).addAthlete(new Athlete("Иванов А.П.", 22, "Беларусь", "Легкая атлетика", 5, 3));
    committee.getEvent(0).addAthlete(new Athlete("Смирнов Д.О.", 19, "Россия", "Плавание", 3, 1));
    committee.getEvent(0).addAthlete(new Athlete("Кузнецов М.А.", 25, "Казахстан", "Гимнастика", 7, 2));
    committee.getEvent(0).addAthlete(new Athlete("Попов Н.С.", 21, "Беларусь", "Легкая атлетика", 4, 1));

    committee.getEvent(0).addCoach(new Coach("Соколов В.И.", 45, "Беларусь", "Плавание", 20));
    committee.getEvent(0).addCoach(new Coach("Морозов А.Г.", 50, "Россия", "Гимнастика", 25));
    committee.getEvent(0).addCoach(new Coach("Волков С.Д.", 39, "Казахстан", "Легкая атлетика", 15));
    committee.getEvent(0).addCoach(new Coach("Зайцев О.Н.", 42, "Беларусь", "Плавание", 18));

    // === Чемпионат области (15.12.2026) ===
    committee.getEvent(1).addAthlete(new Athlete("Лебедев А.В.", 21, "Казахстан", "Плавание", 4, 0));
    committee.getEvent(1).addAthlete(new Athlete("Козлов Р.И.", 24, "Беларусь", "Легкая атлетика", 6, 2));
    committee.getEvent(1).addAthlete(new Athlete("Новиков Е.А.", 23, "Россия", "Гимнастика", 5, 4));
    committee.getEvent(1).addAthlete(new Athlete("Федоров И.Д.", 20, "Беларусь", "Плавание", 2, 1));

    committee.getEvent(1).addCoach(new Coach("Михайлов П.А.", 38, "Беларусь", "Гимнастика", 15));
    committee.getEvent(1).addCoach(new Coach("Беляев Д.С.", 55, "Россия", "Плавание", 30));
    committee.getEvent(1).addCoach(new Coach("Тарасов А.В.", 48, "Казахстан", "Легкая атлетика", 22));
    committee.getEvent(1).addCoach(new Coach("Комаров Ю.А.", 41, "Беларусь", "Гимнастика", 17));

    // === Первенство республики (25.12.2026) ===
    committee.getEvent(2).addAthlete(new Athlete("Орлов В.Ю.", 20, "Россия", "Гимнастика", 2, 5));
    committee.getEvent(2).addAthlete(new Athlete("Киселев Т.Р.", 22, "Беларусь", "Легкая атлетика", 5, 3));
    committee.getEvent(2).addAthlete(new Athlete("Макаров Г.Д.", 19, "Казахстан", "Плавание", 3, 2));
    committee.getEvent(2).addAthlete(new Athlete("Андреев М.С.", 24, "Россия", "Плавание", 6, 1));

    committee.getEvent(2).addCoach(new Coach("Ковалев И.В.", 50, "Казахстан", "Легкая атлетика", 25));
    committee.getEvent(2).addCoach(new Coach("Григорьев А.П.", 47, "Беларусь", "Плавание", 21));
    committee.getEvent(2).addCoach(new Coach("Степанов Р.В.", 35, "Россия", "Гимнастика", 12));
    committee.getEvent(2).addCoach(new Coach("Николаев А.В.", 52, "Казахстан", "Легкая атлетика", 28));
}

// ==================== ОСНОВНАЯ ПРОГРАММА ====================

int askEventIndex(const SportsCommittee& committee) {
    cout << "\n  Доступные соревнования:\n";
    for (size_t i = 0; i < committee.getEvents().size(); ++i) {
        cout << "    " << (i + 1) << ". " << committee.getEvents()[i].getEventName() << "\n";
    }
    while (true) {
        int i = askInt("\n  Выберите номер соревнования (1-3): ");
        if (i >= 1 && i <= 3) return i - 1;
        cout << "  [Ошибка] Неверный номер соревнования.\n";
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");

    SportsCommittee committee;
    committee.clear();

    if (!loadAll(committee)) {
        initData(committee);
        saveAll(committee);
    }

    while (true) {
        cout << "\n========== ЛР №3: КОНТЕЙНЕРЫ STL ==========\n"
            << "  1. Показать все соревнования\n"
            << "  2. Добавить спортсмена\n"
            << "  3. Удалить спортсмена\n"
            << "  4. Редактировать спортсмена\n"
            << "  5. Поиск по фамилии\n"
            << "  6. Сортировка по опыту (убывание)\n"
            << "  7. Фильтр по виду спорта\n"
            << "  8. Управление списком фильтра\n"
            << "  9. Сохранить в файл\n"
            << "  0. Выход\n";
        int choice = askInt("  Выбор: ");

        if (choice == 0) break;

        switch (choice) {
        case 1:
            printAll(cout, committee);
            break;

        case 2: {
            int eIdx = askEventIndex(committee);
            cout << "  Кого добавляем?\n    1. Спортсмена (участника)\n    2. Тренера\n";
            int type = askInt("  Выбор: ");

            string name = askNonEmptyString("ФИО: ");
            int age = askPositiveInt("Возраст: ");
            string country = askNonEmptyString("Страна: ");
            string sport = askNonEmptyString("Вид спорта: ");
            int exp = askPositiveInt("Опыт (лет): ");

            if (type == 1) {
                int medals = askInt("Медали: ");
                committee.getEvent(eIdx).addAthlete(new Athlete(name, age, country, sport, exp, medals));
            }
            else {
                committee.getEvent(eIdx).addCoach(new Coach(name, age, country, sport, exp));
            }
            saveAll(committee);
            cout << "  [OK] Добавлено. Файлы обновлены.\n";
            break;
        }

        case 3: {
            int eIdx = askEventIndex(committee);
            cout << "  Кого удаляем?\n    1. Спортсмена\n    2. Тренера\n";
            int type = askInt("  Выбор: ");
            Event& ev = committee.getEvent(eIdx);

            if (type == 1) {
                if (ev.athleteCount() == 0) { cout << "  Нет спортсменов.\n"; break; }
                cout << "\n  -- Участники --\n";
                printTableHeader(cout);
                for (size_t i = 0; i < ev.athleteCount(); ++i) ev.getAthlete(i)->printRow(cout, static_cast<int>(i + 1));
                int n = askInt("  Номер для удаления: ");
                if (n >= 1 && n <= static_cast<int>(ev.athleteCount())) { ev.removeAthlete(static_cast<size_t>(n - 1)); cout << "  [OK]\n"; }
                else cout << "  [Ошибка] Неверный номер!\n";
            }
            else {
                if (ev.coachCount() == 0) { cout << "  Нет тренеров.\n"; break; }
                cout << "\n  -- Тренеры --\n";
                printTableHeader(cout);
                for (size_t i = 0; i < ev.coachCount(); ++i) ev.getCoach(i)->printRow(cout, static_cast<int>(i + 1));
                int n = askInt("  Номер для удаления: ");
                if (n >= 1 && n <= static_cast<int>(ev.coachCount())) { ev.removeCoach(static_cast<size_t>(n - 1)); cout << "  [OK]\n"; }
                else cout << "  [Ошибка] Неверный номер!\n";
            }
            saveAll(committee);
            break;
        }

        case 4: {
            int eIdx = askEventIndex(committee);
            cout << "  Кого редактируем?\n    1. Спортсмена\n    2. Тренера\n";
            int type = askInt("  Выбор: ");
            Event& ev = committee.getEvent(eIdx);

            Person* old = nullptr;
            bool isAthlete = (type == 1);

            if (isAthlete) {
                if (ev.athleteCount() == 0) { cout << "  Нет спортсменов.\n"; break; }
                cout << "\n  -- Участники --\n";
                printTableHeader(cout);
                for (size_t i = 0; i < ev.athleteCount(); ++i) ev.getAthlete(i)->printRow(cout, static_cast<int>(i + 1));
                int n = askInt("  Номер: ");
                if (n < 1 || n > static_cast<int>(ev.athleteCount())) { cout << "  Неверно!\n"; break; }
                old = ev.getAthlete(static_cast<size_t>(n - 1));
            }
            else {
                if (ev.coachCount() == 0) { cout << "  Нет тренеров.\n"; break; }
                cout << "\n  -- Тренеры --\n";
                printTableHeader(cout);
                for (size_t i = 0; i < ev.coachCount(); ++i) ev.getCoach(i)->printRow(cout, static_cast<int>(i + 1));
                int n = askInt("  Номер: ");
                if (n < 1 || n > static_cast<int>(ev.coachCount())) { cout << "  Неверно!\n"; break; }
                old = ev.getCoach(static_cast<size_t>(n - 1));
            }

            string newName = old->getName();
            int newAge = old->getAge();
            string newCountry = old->getCountry();
            string newSport = old->getSport();
            int newExp = old->getExperience();

            cout << "\n  === РЕДАКТИРОВАНИЕ ===\n";
            if (askBool("Изменить ФИО?")) newName = askNonEmptyString("Новое ФИО: ");
            if (askBool("Изменить возраст?")) newAge = askPositiveInt("Новый возраст: ");
            if (askBool("Изменить страну?")) newCountry = askNonEmptyString("Новая страна: ");
            if (askBool("Изменить вид спорта?")) newSport = askNonEmptyString("Новый вид спорта: ");
            if (askBool("Изменить опыт?")) newExp = askPositiveInt("Новый опыт (лет): ");

            if (isAthlete) {
                Athlete* aOld = dynamic_cast<Athlete*>(old);
                int newMedals = aOld->getMedals();
                if (askBool("Изменить медали?")) newMedals = askInt("Новые медали: ");
                for (size_t i = 0; i < ev.athleteCount(); ++i) {
                    if (ev.getAthlete(i) == old) { ev.setAthlete(i, new Athlete(newName, newAge, newCountry, newSport, newExp, newMedals)); break; }
                }
            }
            else {
                for (size_t i = 0; i < ev.coachCount(); ++i) {
                    if (ev.getCoach(i) == old) { ev.setCoach(i, new Coach(newName, newAge, newCountry, newSport, newExp)); break; }
                }
            }
            cout << "  [OK] Отредактировано.\n";
            saveAll(committee);
            break;
        }

        case 5: {
            string key = askNonEmptyString("  Введите фамилию (или её часть): ");
            bool found = false;
            for (size_t e = 0; e < committee.getEvents().size(); ++e) {
                const Event& ev = committee.getEvents()[e];
                bool eventHasMatches = false;

                for (size_t i = 0; i < ev.athleteCount(); ++i) {
                    if (ev.getAthlete(i)->getName().find(key) != string::npos) {
                        if (!found) { cout << "\n  Результаты поиска:\n"; }
                        if (!eventHasMatches) {
                            cout << "\n  Соревнование: " << ev.getEventName() << " (" << ev.getDate() << ")\n";
                            printTableHeader(cout);
                            eventHasMatches = true;
                        }
                        ev.getAthlete(i)->printRow(cout, static_cast<int>(i + 1));
                        found = true;
                    }
                }
                for (size_t i = 0; i < ev.coachCount(); ++i) {
                    if (ev.getCoach(i)->getName().find(key) != string::npos) {
                        if (!found) { cout << "\n  Результаты поиска:\n"; }
                        if (!eventHasMatches) {
                            cout << "\n  Соревнование: " << ev.getEventName() << " (" << ev.getDate() << ")\n";
                            printTableHeader(cout);
                            eventHasMatches = true;
                        }
                        ev.getCoach(i)->printRow(cout, static_cast<int>(i + 1));
                        found = true;
                    }
                }
            }
            if (!found) cout << "  Ничего не найдено.\n";
            break;
        }

        case 6:
            for (size_t i = 0; i < committee.getEvents().size(); ++i) {
                committee.getEvent(static_cast<int>(i)).sortAthletes();
                committee.getEvent(static_cast<int>(i)).sortCoaches();
            }
            cout << "  [OK] Все отсортировано по опыту (от большего к меньшему).\n";
            break;

        case 7: {
            cout << "\n  Выберите вид спорта:\n    1. Легкая атлетика\n    2. Плавание\n    3. Гимнастика\n    0. Отмена\n";
            int c = askInt("  Выбор: ");
            if (c == 0) break;
            string sport = (c == 1) ? "Легкая атлетика" : (c == 2) ? "Плавание" : "Гимнастика";

            committee.filterBySport(sport);
            cout << "\n  Результаты фильтра (list):\n";
            if (committee.getFilterResults().empty()) cout << "  Ничего не найдено.\n";
            else {
                int i = 1;
                for (const string& s : committee.getFilterResults()) cout << "  " << i++ << ". " << s << "\n";
            }
            break;
        }

        case 8: {
            list<string>& lst = committee.getFilterResults();
            if (lst.empty()) { cout << "  Список пуст. Сначала выполните фильтр (пункт 7).\n"; break; }

            cout << "\n  Текущий список фильтра:\n";
            int i = 1;
            for (const string& s : lst) cout << "  " << i++ << ". " << s << "\n";

            cout << "\n  1. Сортировать\n  2. Удалить строку\n  0. Назад\n";
            int sub = askInt("  Выбор: ");
            if (sub == 1) { lst.sort(); cout << "  [OK]\n"; }
            else if (sub == 2) {
                int n = askInt("  Номер строки: ");
                if (n >= 1 && n <= static_cast<int>(lst.size())) {
                    auto it = lst.begin(); advance(it, static_cast<ptrdiff_t>(n - 1)); lst.erase(it);
                    cout << "  [OK] Удалено.\n";
                }
                else cout << "  [Ошибка]\n";
            }
            break;
        }

        case 9:
            saveAll(committee);
            cout << "  [OK] Сохранено в файлы " << DATA_FILE << " и " << REPORT_FILE << "\n";
            break;
        }
    }
    return 0;
}
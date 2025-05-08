#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <sstream>
#include <locale>
#include <clocale>

void setRussianLocale() {
    std::setlocale(LC_ALL, "ru_RU.UTF-8");
    std::locale::global(std::locale("ru_RU.UTF-8"));
    std::wcout.imbue(std::locale("ru_RU.UTF-8"));
    std::wcin.imbue(std::locale("ru_RU.UTF-8"));
}

class User {
protected:
    std::wstring name;
    int id;
    int accessLevel;

public:
    User(const std::wstring& name, int id, int accessLevel)
        : name(name), id(id), accessLevel(accessLevel) {
        if (name.empty()) {
            throw std::invalid_argument("Имя пользователя не может быть пустым");
        }
        if (accessLevel < 0) {
            throw std::invalid_argument("Уровень доступа не может быть отрицательным");
        }
    }

    virtual ~User() = default;

    std::wstring getName() const { return name; }
    int getId() const { return id; }
    int getAccessLevel() const { return accessLevel; }

    void setName(const std::wstring& newName) {
        if (newName.empty()) {
            throw std::invalid_argument("Имя пользователя не может быть пустым");
        }
        name = newName;
    }

    void setAccessLevel(int newLevel) {
        if (newLevel < 0) {
            throw std::invalid_argument("Уровень доступа не может быть отрицательным");
        }
        accessLevel = newLevel;
    }

    virtual void displayInfo() const {
        std::wcout << L"Имя: " << name << L", ID: " << id
            << L", Уровень доступа: " << accessLevel;
    }

    virtual std::wstring toFileString() const {
        return L"USER," + name + L"," + std::to_wstring(id) + L"," + std::to_wstring(accessLevel);
    }
};

class Student : public User {
private:
    std::wstring group;

public:
    Student(const std::wstring& name, int id, int accessLevel, const std::wstring& group)
        : User(name, id, accessLevel), group(group) {
        if (group.empty()) {
            throw std::invalid_argument("Группа не может быть пустой");
        }
    }

    std::wstring getGroup() const { return group; }
    void setGroup(const std::wstring& newGroup) {
        if (newGroup.empty()) {
            throw std::invalid_argument("Группа не может быть пустой");
        }
        group = newGroup;
    }

    void displayInfo() const override {
        User::displayInfo();
        std::wcout << L", Группа: " << group << L" (Студент)";
    }

    std::wstring toFileString() const override {
        return L"STUDENT," + name + L"," + std::to_wstring(id) + L","
            + std::to_wstring(accessLevel) + L"," + group;
    }
};

class Teacher : public User {
private:
    std::wstring department;

public:
    Teacher(const std::wstring& name, int id, int accessLevel, const std::wstring& department)
        : User(name, id, accessLevel), department(department) {
        if (department.empty()) {
            throw std::invalid_argument("Кафедра не может быть пустой");
        }
    }

    std::wstring getDepartment() const { return department; }
    void setDepartment(const std::wstring& newDepartment) {
        if (newDepartment.empty()) {
            throw std::invalid_argument("Кафедра не может быть пустой");
        }
        department = newDepartment;
    }

    void displayInfo() const override {
        User::displayInfo();
        std::wcout << L", Кафедра: " << department << L" (Преподаватель)";
    }

    std::wstring toFileString() const override {
        return L"TEACHER," + name + L"," + std::to_wstring(id) + L","
            + std::to_wstring(accessLevel) + L"," + department;
    }
};

class Administrator : public User {
private:
    std::wstring position;

public:
    Administrator(const std::wstring& name, int id, int accessLevel, const std::wstring& position)
        : User(name, id, accessLevel), position(position) {
        if (position.empty()) {
            throw std::invalid_argument("Должность не может быть пустой");
        }
    }

    std::wstring getPosition() const { return position; }
    void setPosition(const std::wstring& newPosition) {
        if (newPosition.empty()) {
            throw std::invalid_argument("Должность не может быть пустой");
        }
        position = newPosition;
    }

    void displayInfo() const override {
        User::displayInfo();
        std::wcout << L", Должность: " << position << L" (Директор кафедры)";
    }

    std::wstring toFileString() const override {
        return L"ADMIN," + name + L"," + std::to_wstring(id) + L","
            + std::to_wstring(accessLevel) + L"," + position;
    }
};

// Класс ресурса
class Resource {
private:
    std::wstring name;
    int requiredAccessLevel;

public:
    Resource(const std::wstring& name, int requiredAccessLevel)
        : name(name), requiredAccessLevel(requiredAccessLevel) {
        if (name.empty()) {
            throw std::invalid_argument("Название ресурса не может быть пустым");
        }
        if (requiredAccessLevel < 0) {
            throw std::invalid_argument("Требуемый уровень доступа не может быть отрицательным");
        }
    }

    std::wstring getName() const { return name; }
    int getRequiredAccessLevel() const { return requiredAccessLevel; }

    void setName(const std::wstring& newName) {
        if (newName.empty()) {
            throw std::invalid_argument("Название ресурса не может быть пустым");
        }
        name = newName;
    }

    void setRequiredAccessLevel(int newLevel) {
        if (newLevel < 0) {
            throw std::invalid_argument("Требуемый уровень доступа не может быть отрицательным");
        }
        requiredAccessLevel = newLevel;
    }

    bool checkAccess(const User& user) const {
        return user.getAccessLevel() >= requiredAccessLevel;
    }

    std::wstring toFileString() const {
        return name + L"," + std::to_wstring(requiredAccessLevel);
    }
};

template<typename T>
class AccessControlSystem {
private:
    std::vector<std::unique_ptr<User>> users;
    std::vector<T> resources;

public:
    void addUser(std::unique_ptr<User> user) {
        users.push_back(std::move(user));
    }

    void addResource(const T& resource) {
        resources.push_back(resource);
    }

    bool checkAccess(int userId, const std::wstring& resourceName) const {
        auto userIt = std::find_if(users.begin(), users.end(),
            [userId](const std::unique_ptr<User>& user) { return user->getId() == userId; });

        auto resourceIt = std::find_if(resources.begin(), resources.end(),
            [resourceName](const T& resource) { return resource.getName() == resourceName; });

        if (userIt == users.end()) {
            throw std::runtime_error("Пользователь не найден");
        }
        if (resourceIt == resources.end()) {
            throw std::runtime_error("Ресурс не найден");
        }

        return resourceIt->checkAccess(**userIt);
    }

    void displayAllUsers() const {
        for (const auto& user : users) {
            user->displayInfo();
            std::wcout << std::endl;
        }
    }

    void displayAllResources() const {
        for (const auto& resource : resources) {
            std::wcout << L"Ресурс: " << resource.getName()
                << L", Требуемый уровень доступа: " << resource.getRequiredAccessLevel()
                << std::endl;
        }
    }

    void saveToFile(const std::wstring& filename) const {
        std::wofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Не удалось открыть файл для записи");
        }

        for (const auto& user : users) {
            file << user->toFileString() << std::endl;
        }

        file << L"---RESOURCES---" << std::endl;
        for (const auto& resource : resources) {
            file << resource.toFileString() << std::endl;
        }

        file.close();
    }

    void loadFromFile(const std::wstring& filename) {
        std::wifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Не удалось открыть файл для чтения");
        }

        users.clear();
        resources.clear();

        std::wstring line;
        bool readingResources = false;

        while (std::getline(file, line)) {
            if (line.empty()) continue;

            if (line == L"---RESOURCES---") {
                readingResources = true;
                continue;
            }

            if (!readingResources) {
                std::wistringstream iss(line);
                std::wstring type;
                std::getline(iss, type, L',');

                if (type == L"USER") {
                    std::wstring name;
                    int id, accessLevel;
                    wchar_t comma;
                    std::getline(iss, name, L',');
                    iss >> id >> comma >> accessLevel;
                    users.push_back(std::make_unique<User>(name, id, accessLevel));
                }
                else if (type == L"STUDENT") {
                    std::wstring name, group;
                    int id, accessLevel;
                    wchar_t comma;
                    std::getline(iss, name, L',');
                    iss >> id >> comma >> accessLevel >> comma;
                    std::getline(iss, group);
                    users.push_back(std::make_unique<Student>(name, id, accessLevel, group));
                }
                else if (type == L"TEACHER") {
                    std::wstring name, department;
                    int id, accessLevel;
                    wchar_t comma;
                    std::getline(iss, name, L',');
                    iss >> id >> comma >> accessLevel >> comma;
                    std::getline(iss, department);
                    users.push_back(std::make_unique<Teacher>(name, id, accessLevel, department));
                }
                else if (type == L"ADMIN") {
                    std::wstring name, position;
                    int id, accessLevel;
                    wchar_t comma;
                    std::getline(iss, name, L',');
                    iss >> id >> comma >> accessLevel >> comma;
                    std::getline(iss, position);
                    users.push_back(std::make_unique<Administrator>(name, id, accessLevel, position));
                }
            }
            else {
                std::wistringstream iss(line);
                std::wstring name;
                int requiredAccessLevel;
                wchar_t comma;
                std::getline(iss, name, L',');
                iss >> requiredAccessLevel;
                resources.emplace_back(name, requiredAccessLevel);
            }
        }

        file.close();
    }

    std::vector<User*> findUsersByName(const std::wstring& name) const {
        std::vector<User*> result;
        for (const auto& user : users) {
            if (user->getName() == name) {
                result.push_back(user.get());
            }
        }
        return result;
    }

    User* findUserById(int id) const {
        auto it = std::find_if(users.begin(), users.end(),
            [id](const std::unique_ptr<User>& user) { return user->getId() == id; });
        return (it != users.end()) ? it->get() : nullptr;
    }

    void sortUsersByAccessLevel() {
        std::sort(users.begin(), users.end(),
            [](const std::unique_ptr<User>& a, const std::unique_ptr<User>& b) {
                return a->getAccessLevel() < b->getAccessLevel();
            });
    }

    void sortUsersById() {
        std::sort(users.begin(), users.end(),
            [](const std::unique_ptr<User>& a, const std::unique_ptr<User>& b) {
                return a->getId() < b->getId();
            });
    }
};

void demonstrateAccessControlSystem() {
    AccessControlSystem<Resource> system;

    try {
        system.addUser(std::make_unique<Student>(L"Ростислав Долгополов", 1, 1, L"Группа 03"));
        system.addUser(std::make_unique<Teacher>(L"Тащилин Максим Васильевич", 2, 3, L"Кафедра Т-университет"));
        system.addUser(std::make_unique<Administrator>(L"Маркарьян Юлия Артёмовна", 3, 5, L"Директор"));
        system.addResource(Resource(L"Аудитория 477", 1));
        system.addResource(Resource(L"Лаборатория физики", 3));
        system.addResource(Resource(L"Библиотека", 2));
        system.addResource(Resource(L"Серверная", 5));

        std::wcout << L"Все пользователи:" << std::endl;
        system.displayAllUsers();
        std::wcout << std::endl;
        std::wcout << L"Все ресурсы:" << std::endl;
        system.displayAllResources();
        std::wcout << std::endl;
        std::wcout << L"Проверка доступа:" << std::endl;
        std::wcout << L"Студент в аудитории 477: "
            << (system.checkAccess(1, L"Аудитория 477") ? L"Доступ разрешен" : L"Доступ запрещен") << std::endl;
        std::wcout << L"Студент в лаборатории физики: "
            << (system.checkAccess(1, L"Лаборатория физики") ? L"Доступ разрешен" : L"Доступ запрещен") << std::endl;
        std::wcout << L"Преподаватель в лаборатории физики: "
            << (system.checkAccess(2, L"Лаборатория физики") ? L"Доступ разрешен" : L"Доступ запрещен") << std::endl;
        std::wcout << L"Директор в серверной: "
            << (system.checkAccess(3, L"Серверная") ? L"Доступ разрешен" : L"Доступ запрещен") << std::endl;
        std::wcout << std::endl;
        std::wcout << L"Поиск пользователя по ID 2:" << std::endl;
        User* foundUser = system.findUserById(2);
        if (foundUser) {
            foundUser->displayInfo();
            std::wcout << std::endl;
        }
        std::wcout << std::endl;
        std::wcout << L"Пользователи, отсортированные по уровню доступа:" << std::endl;
        system.sortUsersByAccessLevel();
        system.displayAllUsers();
        std::wcout << std::endl;
        const std::wstring filename = L"university_access_system.txt";
        system.saveToFile(filename);
        std::wcout << L"Система сохранена в файл: " << filename << std::endl;
        AccessControlSystem<Resource> newSystem;
        newSystem.loadFromFile(filename);
        std::wcout << L"Система загружена из файла:" << std::endl;
        newSystem.displayAllUsers();
        newSystem.displayAllResources();

    }
    catch (const std::exception& e) {
        std::wcerr << L"Ошибка: " << e.what() << std::endl;
    }
}

int main() {
    setRussianLocale();
    demonstrateAccessControlSystem();
    return 0;
}
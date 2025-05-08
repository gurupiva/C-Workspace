#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

class Entity {
protected:
    std::string name;
    int health;
    int level;

public:
    Entity(const std::string& n, int h, int l) : name(n), health(h), level(l) {}
    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health << ", Level: " << level << " " << std::endl;
    }
    virtual std::string serialize() const = 0;
    virtual ~Entity() {}
    int getLevel() const { return level; }
};

class Player : public Entity {
private:
    std::string clas;

public:
    Player(const std::string& n, int h, int l, const std::string& c)
        : Entity(n, h, l), clas(c) {
    }

    void displayInfo() const override {
        Entity::displayInfo();
        std::cout << "Class: " << clas << std::endl;
    }

    std::string serialize() const override {
        return name + " " + std::to_string(health) + " " + std::to_string(getLevel()) + " " + clas;
    }
};


class Enemy : public Entity {

public:
    Enemy(const std::string& n, int h, int l)
        : Entity(n, h, l) {
    }

    void displayInfo() const override {
        Entity::displayInfo();
    }

    std::string serialize() const override {
        return  name + " " + std::to_string(health) + " " + std::to_string(getLevel());
    }
};

template <typename T>
class GameManager {
private:
    std::vector<T> entities;

public:
    void addEntity(const T& entity) {
        entities.push_back(entity);
    }

    void displayAll() const {
        for (const auto& entity : entities) {
            entity->displayInfo();
        }
    }

    const std::vector<T>& getEntities() const {
        return entities;
    }

    void clear() {
        entities.clear();
    }
};

// Функция сохранения
void saveToFile(const GameManager<Entity*>& manager, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file for writing.");
    }
    for (const auto& entity : manager.getEntities()) {
        if (dynamic_cast<Player*>(entity)) {
            file << "Player ";
        }
        else if (dynamic_cast<Enemy*>(entity)) {
            file << "Enemy ";
        }
        file << entity->serialize() << std::endl;
    }
}


void loadFromFile(GameManager<Entity*>& manager, const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file for reading.");
    }

    std::string type, name;
    int health, level;
    manager.clear();

    while (file >> type >> name >> health >> level) {
        if (type == "Player") {
            std::string clas;
            file >> clas;
            manager.addEntity(new Player(name, health, level, clas));
        }
        else if (type == "Enemy") {
            manager.addEntity(new Enemy(name, health, level));
        }
    }
}

// main
int main() {
    try {
        GameManager<Entity*> manager;
        manager.addEntity(new Player("Hero", 100, 1, "Human"));
        manager.addEntity(new Enemy("Goblin", 50, 2));
        manager.addEntity(new Player("Fire_Mage", 80, 3, "Mage"));
        manager.addEntity(new Enemy("Dragon", 200, 5));

        std::cout << "Entities:\n";
        manager.displayAll();

        saveToFile(manager, "game1.txt");

        GameManager<Entity*> loadedManager;
        loadFromFile(loadedManager, "game1.txt");

        std::cout << "\nLoaded entities from file:\n";
        loadedManager.displayAll();

        for (auto& entity : manager.getEntities()) {
            delete entity;
        }
        for (auto& entity : loadedManager.getEntities()) {
            delete entity;
        }

    }
    catch (const std::exception& e) {
        std::cerr << "\nException: " << e.what() << std::endl;
    }

    return 0;
}
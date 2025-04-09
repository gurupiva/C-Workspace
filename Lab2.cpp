#include <iostream>
#include <string>

class Weapon {
private:
    std::string name;
    int damage;
    float weight;

public:
    // Конструктор
    Weapon(const std::string& n, int d, float w)
        : name(n), damage(d), weight(w) {
        std::cout << "Weapon " << name << " created!\n";
    }

    // Деструктор
    ~Weapon() {
        std::cout << "Weapon " << name << " destroyed!\n";
    }

    // Метод для вывода информации
    void displayInfo() const {
        std::cout << "Weapon: " << name
            << ", Damage: " << damage
            << ", Weight: " << weight << "kg\n";
    }
};

class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
        std::cout << "Character " << name << " created!\n";
    }

    ~Character() {
        std::cout << "Character " << name << " destroyed!\n";
    }

    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
};

class Monster {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Monster(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
        std::cout << "Monster " << name << " created!\n";
    }

    ~Monster() {
        std::cout << "Monster " << name << " destroyed!\n";
    }

    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
};

int main() {
    Weapon sword("Excalibur", 25, 3.5f);
    Weapon bow("Longbow", 15, 1.2f);
    Weapon axe("Battle Axe", 30, 5.0f);

    std::cout << "\nWeapon Information:\n";
    sword.displayInfo();
    bow.displayInfo();
    axe.displayInfo();

    Character hero("Arthur", 100, 20, 10);
    Monster dragon("Smaug", 200, 30, 20);

    std::cout << "\nCharacter Information:\n";
    hero.displayInfo();
    dragon.displayInfo();

    return 0;
}
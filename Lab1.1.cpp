#include <iostream>
#include <string>

class Character {
private:
    std::string name;  // Приватное поле: имя персонажа
    int health;        // Приватное поле: уровень здоровья
    int attack;        // Приватное поле: уровень атаки
    int defense;       // Приватное поле: уровень защиты
    const int maxHealth = 100; // Максимальное здоровье

public:
    // Конструктор для инициализации данных
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {}

    // Метод для получения уровня здоровья
    int getHealth() const {
        return health;
    }

    // Метод для вывода информации о персонаже
    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }

    // Метод для атаки другого персонажа
    void attackEnemy(Character& enemy) {
        int damage = attack - enemy.defense;
        if (damage > 0) {
            enemy.takeDamage(damage);
            std::cout << name << " attacks " << enemy.name << " for " << damage << " damage!" << std::endl;
        }
        else {
            std::cout << name << " attacks " << enemy.name << ", but it has no effect!" << std::endl;
        }
    }

    // Метод для лечения персонажа (не выше maxHealth)
    void heal(int amount) {
        if (amount > 0) {
            health += amount;
            if (health > maxHealth) {
                health = maxHealth;
            }
            std::cout << name << " heals for " << amount << " HP. Current HP: " << health << std::endl;
        }
    }

    // Метод для получения урона (не ниже 0)
    void takeDamage(int amount) {
        if (amount > 0) {
            health -= amount;
            if (health < 0) {
                health = 0;
            }
        }
    }
};

int main() {
    Character hero("Hero", 100, 20, 10);
    Character monster("Goblin", 50, 15, 5);

    hero.displayInfo();
    monster.displayInfo();

    hero.attackEnemy(monster);
    monster.displayInfo();

    monster.attackEnemy(hero);
    hero.displayInfo();

    hero.heal(30);
    hero.displayInfo();

    hero.takeDamage(50);
    hero.displayInfo();

    hero.heal(100);
    hero.displayInfo();

    return 0;
}
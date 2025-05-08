#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>

class Entity {
protected:
    std::string name;
    int health;
    int level;
    int attack;
    int defense;

public:
    Entity(const std::string& n, int h, int a, int d) : name(n), health(h), attack(a), defense(d) {}
    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health << ", Attack: " << attack << ", Defense: " << defense << " " << std::endl;
    }
    virtual std::string serialize() const = 0;
    virtual ~Entity() {}
    int getHealth() const { return health; }
    void setHealth(int h) { health = h; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
    const std::string& getName() const { return name; }
    virtual std::string getType() const { return "Entity"; }
};

class Player : public Entity {
private:
    std::string clas;
    int healAmount;

public:
    Player(const std::string& n, int h, int a, int d, const std::string& c, int heal)
        : Entity(n, h, a, d), clas(c), healAmount(heal) {
    }
    void displayInfo() const override {
        Entity::displayInfo();
        std::cout << "Class: " << clas << std::endl;
    }

    std::string serialize() const override {
        return name + " " + std::to_string(health) + " " + " " + clas;
    }
    int getHealAmount() const { return healAmount; }
    std::string getType() const override { return "Player"; }
};


class Enemy : public Entity {
private:
    std::string enemyType;

public:
    Enemy(const std::string& n, int h, int a, int d, const std::string& type)
        : Entity(n, h, a, d), enemyType(type) {
    }

    void displayInfo() const override {
        Entity::displayInfo();
        std::cout << "Type: " << enemyType << std::endl;
    }

    std::string serialize() const override {
        return  name + " " + std::to_string(health);
    }
    const std::string& getEnemyType() const { return enemyType; }
    std::string getType() const override { return "Enemy"; }
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

bool combat(Player* player, Enemy* enemy, std::mutex& mtx) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 20);

    while (player->getHealth() > 0 && enemy->getHealth() > 0) {
        // Игрок атакует
        {
            std::lock_guard<std::mutex> lock(mtx);
            int damage = std::max(0, player->getAttack() - enemy->getDefense());
            int enemyHealth = enemy->getHealth();
            enemyHealth -= damage;
            enemyHealth = std::max(0, enemyHealth);
            enemy->setHealth(enemyHealth);
            std::cout << player->getName() << " attacks " << enemy->getName() << " for " << damage << " damage. "
                << enemy->getName() << " HP: " << enemy->getHealth() << std::endl;
        }

        if (enemy->getHealth() <= 0) {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << enemy->getName() << " is defeated!" << std::endl;
            int healAmount = player->getHealAmount();
            int playerHealth = player->getHealth();
            playerHealth = std::min(100, playerHealth + healAmount);
            player->setHealth(playerHealth);
            std::cout << player->getName() << " heals for " << healAmount << ". " << player->getName() << " HP: " << player->getHealth() << std::endl;
            return true;
        }

        // Враг атакует
        {
            std::lock_guard<std::mutex> lock(mtx);
            int damage = std::max(0, enemy->getAttack() - player->getDefense());
            int playerHealth = player->getHealth();
            playerHealth -= damage;
            playerHealth = std::max(0, playerHealth);
            player->setHealth(playerHealth);
            std::cout << enemy->getName() << " attacks " << player->getName() << " for " << damage << " damage. "
                << player->getName() << " HP: " << player->getHealth() << std::endl;
        }

        if (player->getHealth() <= 0) {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << player->getName() << " is defeated!" << std::endl;
            return false;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(distrib(gen) * 100));
    }

    return false;
}

int main() {
    std::cout << "Start Game" << std::endl;
    try {
        Player* player = new Player("Hero", 100, 13, 6, "Warrior", 20);

        std::vector<Enemy*> enemies;
        enemies.push_back(new Enemy("Goblin", 50, 10, 6, "Melee"));
        enemies.push_back(new Enemy("Dragon", 70, 10, 8, "Fire"));
        enemies.push_back(new Enemy("Spider", 30, 8, 1, "Poison"));
        enemies.push_back(new Enemy("Skeleton", 40, 8, 3, "Undead"));
        enemies.push_back(new Enemy("Orc", 45, 9, 5, "Melee"));



        std::mutex mtx;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, enemies.size() - 1);

        bool gameRunning = true;
        while (gameRunning) {
            int enemyIndex = distrib(gen);
            Enemy* enemy = enemies[enemyIndex];

            std::cout << "\nA wild " << enemy->getName() << " appears!" << std::endl;
            std::thread combatThread(combat, player, enemy, std::ref(mtx));
            combatThread.join();

            if (player->getHealth() <= 0) {
                gameRunning = false;
                std::cout << "Game Over!" << std::endl;
            }
            else
            {
                //Respawn Enemy:
                enemy->setHealth(50 + (rand() % 100));
                std::this_thread::sleep_for(std::chrono::seconds(3));
            }
        }

        std::cout << "\nFinal Result:" << std::endl;
        player->displayInfo();

        delete player;
        for (Enemy* enemy : enemies) {
            delete enemy;
        }
        enemies.clear();

    }
    catch (const std::exception& e) {
        std::cerr << "\nException: " << e.what() << std::endl;
    }

    return 0;
}
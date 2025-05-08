#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <ctime>
#include <memory>
#include <random>
#include <algorithm>

using namespace std;
template<typename T>
class Logger {
    ofstream logFile;
public:
    Logger(const string& filename) {
        logFile.open(filename, ios::app);
        if (!logFile) throw runtime_error("Cannot open log file");
    }

    void log(const T& message) {
        time_t now = time(0);
        char timestamp[26];
        ctime_s(timestamp, sizeof(timestamp), &now);
        logFile << "[" << timestamp << "] " << message << endl;
    }
};

class Item {
    string name;
    string type;
    int value;
public:
    Item(const string& n, const string& t, int v) : name(n), type(t), value(v) {}

    string getName() const { return name; }
    string getType() const { return type; }
    int getValue() const { return value; }
};

class Inventory {
    vector<Item> items;
public:
    void addItem(const Item& item) {
        if (items.size() >= 20) {
            throw runtime_error("Inventory is full");
        }
        items.push_back(item);
    }

    void removeItem(int index) {
        if (index < 0 || index >= static_cast<int>(items.size())) {
            throw out_of_range("Invalid item index");
        }
        items.erase(items.begin() + index);
    }

    void display() const {
        if (items.empty()) {
            cout << "Inventory is empty" << endl;
            return;
        }
        for (size_t i = 0; i < items.size(); ++i) {
            cout << i + 1 << ". " << items[i].getName()
                << " (" << items[i].getType()
                << ", Value: " << items[i].getValue() << ")" << endl;
        }
    }

    const vector<Item>& getItems() const { return items; }
};

class Character;
class Monster {
protected:
    string name;
    int health;
    int attack;
    int defense;
public:
    Monster(const string& n, int h, int a, int d) : name(n), health(h), attack(a), defense(d) {}
    virtual ~Monster() = default;

    virtual void attackTarget(Character& target);

    bool isAlive() const { return health > 0; }
    string getName() const { return name; }
    int getHealth() const { return health; }
    int getDefense() const { return defense; }
    void takeDamage(int damage) {
        health = max(0, health - damage);
    }

    virtual void displayInfo() const {
        cout << name << " [HP: " << health << ", ATK: " << attack << ", DEF: " << defense << "]" << endl;
    }

    virtual Item getLoot() const = 0;
};

class Character {
    string name;
    int health;
    int maxHealth;
    int attack;
    int defense;
    int level;
    int experience;
    Inventory inventory;
    Logger<string> logger;

public:
    Character(const string& n, int h, int a, int d)
        : name(n), health(h), maxHealth(h), attack(a), defense(d), level(1), experience(0), logger("game_log.txt") {
        logger.log("Character " + name + " created");
    }

    void takeDamage(int damage) {
        health = max(0, health - damage);
        if (health == 0) {
            throw runtime_error(name + " has been defeated!");
        }
    }

    void attackEnemy(Monster& enemy) {
        int damage = max(1, attack - enemy.getDefense());
        enemy.takeDamage(damage);
        logger.log(name + " attacks " + enemy.getName() + " for " + to_string(damage) + " damage");
        cout << name << " attacks " << enemy.getName() << " for " << damage << " damage!" << endl;

        if (!enemy.isAlive()) {
            gainExperience(50);
            try {
                Item loot = enemy.getLoot();
                inventory.addItem(loot);
                cout << "Got " << loot.getName() << " from " << enemy.getName() << endl;
                logger.log("Obtained " + loot.getName() + " from " + enemy.getName());
            }
            catch (const exception& e) {
                logger.log("Error getting loot: " + string(e.what()));
            }
        }
    }

    void heal(int amount) {
        health = min(health + amount, maxHealth);
        cout << name << " heals for " << amount << " HP!" << endl;
        logger.log(name + " healed for " + to_string(amount) + " HP");
    }

    void gainExperience(int exp) {
        experience += exp;
        if (experience >= 100) {
            level++;
            experience -= 100;
            maxHealth += 10;
            attack += 2;
            defense += 1;
            cout << name << " leveled up to level " << level << "!" << endl;
            logger.log(name + " leveled up to " + to_string(level));
        }
    }

    void displayInfo() const {
        cout << "Name: " << name << ", HP: " << health << "/" << maxHealth
            << ", ATK: " << attack << ", DEF: " << defense
            << ", LVL: " << level << ", EXP: " << experience << "/100" << endl;
    }

    void showInventory() {
        cout << "=== Inventory ===" << endl;
        inventory.display();
        cout << "================" << endl;
    }

    void useItem(int index) {
        try {
            const vector<Item>& items = inventory.getItems();
            if (index < 0 || index >= static_cast<int>(items.size())) {
                throw out_of_range("Invalid item index");
            }

            const Item& item = items[index];
            if (item.getType() == "potion") {
                heal(item.getValue());
                inventory.removeItem(index);
                logger.log(name + " used " + item.getName());
            }
            else {
                cout << "Can't use this item" << endl;
            }
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
            logger.log("Error using item: " + string(e.what()));
        }
    }

    void saveGame(const string& filename) {
        ofstream file(filename);
        if (!file) {
            throw runtime_error("Failed to save game");
        }

        file << name << endl
            << health << endl
            << maxHealth << endl
            << attack << endl
            << defense << endl
            << level << endl
            << experience << endl;

        logger.log("Game saved to " + filename);
    }

    void loadGame(const string& filename) {
        ifstream file(filename);
        if (!file) {
            throw runtime_error("Failed to load game");
        }

        file >> name >> health >> maxHealth >> attack >> defense >> level >> experience;
        logger.log("Game loaded from " + filename);
    }

    string getName() const { return name; }
    int getHealth() const { return health; }
    int getDefense() const { return defense; }
};

void Monster::attackTarget(Character& target) {
    int damage = max(1, attack - target.getDefense());
    target.takeDamage(damage);
    cout << name << " attacks " << target.getName() << " for " << damage << " damage!" << endl;
}

class Goblin : public Monster {
public:
    Goblin() : Monster("Goblin", 30, 8, 3) {}
    Item getLoot() const override { return Item("Goblin Ear", "material", 5); }
};

class Dragon : public Monster {
public:
    Dragon() : Monster("Dragon", 100, 20, 10) {}
    Item getLoot() const override { return Item("Dragon Scale", "material", 20); }
};

class Skeleton : public Monster {
public:
    Skeleton() : Monster("Skeleton", 40, 12, 5) {}
    Item getLoot() const override { return Item("Bone", "material", 8); }
};

class Game {
    unique_ptr<Character> player;
    vector<unique_ptr<Monster>> monsters;
    random_device rd;
    mt19937 gen;

    void generateMonsters() {
        monsters.clear();
        monsters.push_back(make_unique<Goblin>());
        monsters.push_back(make_unique<Skeleton>());
        monsters.push_back(make_unique<Dragon>());
        shuffle(monsters.begin(), monsters.end(), gen);
    }

    void clearScreen() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

public:
    Game() : gen(rd()) {
        string name;
        cout << "Enter your name: ";
        getline(cin, name);
        player = make_unique<Character>(name, 100, 10, 5);
        generateMonsters();
    }

    void startBattle() {
        if (monsters.empty()) {
            cout << "No monsters to fight!" << endl;
            return;
        }

        auto& monster = monsters.back();
        cout << "A wild " << monster->getName() << " appears!" << endl;

        while (player->getHealth() > 0 && monster->isAlive()) {
            cout << "\n=== Battle ===" << endl;
            player->displayInfo();
            monster->displayInfo();
            cout << "1. Attack\n2. Use Item\n3. Flee\nChoice: ";

            int choice;
            cin >> choice;
            cin.ignore();

            try {
                switch (choice) {
                case 1:
                    player->attackEnemy(*monster);
                    if (monster->isAlive()) {
                        monster->attackTarget(*player);
                    }
                    break;
                case 2:
                    player->showInventory();
                    cout << "Enter item number to use: ";
                    int itemNum;
                    cin >> itemNum;
                    cin.ignore();
                    player->useItem(itemNum - 1);
                    break;
                case 3:
                    cout << "You fled from battle!" << endl;
                    return;
                default:
                    cout << "Invalid choice!" << endl;
                }
            }
            catch (const exception& e) {
                cout << e.what() << endl;
                break;
            }
        }

        if (!monster->isAlive()) {
            monsters.pop_back();
        }
    }

    void mainMenu() {
        while (true) {
            cout << "\n=== Main Menu ===" << endl;
            cout << "1. Battle\n2. View Stats\n3. View Inventory\n4. Save Game\n5. Load Game\n6. Exit\nChoice: ";

            int choice;
            cin >> choice;
            cin.ignore();

            try {
                switch (choice) {
                case 1:
                    startBattle();
                    if (monsters.empty()) {
                        cout << "You defeated all monsters!" << endl;
                        generateMonsters();
                    }
                    break;
                case 2:
                    player->displayInfo();
                    break;
                case 3:
                    player->showInventory();
                    break;
                case 4:
                    player->saveGame("save.txt");
                    cout << "Game saved!" << endl;
                    break;
                case 5:
                    player->loadGame("save.txt");
                    cout << "Game loaded!" << endl;
                    break;
                case 6:
                    return;
                default:
                    cout << "Invalid choice!" << endl;
                }
            }
            catch (const exception& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
    }
};

int main() {
    try {
        Game game;
        game.mainMenu();
    }
    catch (const exception& e) {
        cerr << "Fatal error: " << e.what() << endl;
        return 1;
    }
    return 0;
}

#include <iostream>
#include <string>
class Weapon {
private:
    std::string name;
    int damage;
public:
    Weapon(const std::string& n, int d) : name(n), damage(d) {}
    Weapon operator+(const Weapon& other) const {
        return Weapon(name + "+" + other.name, damage + other.damage);
    }
    bool operator>(const Weapon& other) const {
        return damage > other.damage;
    }
    friend std::ostream& operator<<(std::ostream& os, const Weapon& w) {
        os << "Weapon: " << w.name << ", Damage: " << w.damage;
        return os;
    }
};
int main() {
    Weapon sword("Sword", 30);
    Weapon bow("Bow", 25);
    Weapon axe("Axe", 40);
    Weapon combinedWeapon = sword + bow;
    std::cout << "Combined weapon: " << combinedWeapon << std::endl;
    if (sword > bow) {
        std::cout << sword << " is stronger than " << bow << std::endl;
    }
    else {
        std::cout << bow << " is stronger than " << sword << std::endl;
    }
    if (axe > combinedWeapon) {
        std::cout << axe << " is stronger than " << combinedWeapon << std::endl;
    }
    return 0;
}
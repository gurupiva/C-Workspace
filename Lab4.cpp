#include <iostream>
#include <memory>
#include <string>
class Inventory {
private:
    std::unique_ptr<std::string[]> items;
    size_t capacity;
    size_t count;
public:
    Inventory() : items(nullptr), capacity(0), count(0) {}
    void addItem(const std::string& item) {
        if (count >= capacity) {
            size_t new_capacity = (capacity == 0) ? 1 : capacity * 2;
            std::unique_ptr<std::string[]> new_items = std::make_unique<std::string[]>(new_capacity);
            for (size_t i = 0; i < count; ++i) {
                new_items[i] = items[i];
            }
            items = std::move(new_items);
            capacity = new_capacity;
        }
        items[count] = item;
        ++count;
    }
    void displayInventory() const {
        if (count == 0) {
            std::cout << "Inventory is empty." << std::endl;
            return;
        }
        std::cout << "Inventory items:" << std::endl;
        for (size_t i = 0; i < count; ++i) {
            std::cout << "- " << items[i] << std::endl;
        }
    }
};
int main() {
    Inventory inv;
    inv.addItem("Sword");
    inv.addItem("Health Potion");
    inv.addItem("Shield");
    inv.addItem("Mana Potion");
    inv.displayInventory();
    return 0;
}
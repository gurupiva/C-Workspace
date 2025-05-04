#include <iostream>
#include <deque>
#include <stdexcept>
#include <locale.h> // Для работы с русской локалью
template <typename T>
class Queue {
private:
    std::deque<T> elements;
public:
    void push(const T& item) {
        elements.push_back(item);
    }
    void pop() {
        if (elements.empty()) {
            throw std::runtime_error("Очередь пуста. Нельзя удалить элемент.");
        }
        elements.pop_front();
    }
    T& front() {
        if (elements.empty()) {
            throw std::runtime_error("Очередь пуста. Нет элементов для отображения.");
        }
        return elements.front();
    }
    bool empty() const {
        return elements.empty();
    }
};
int main() {
    setlocale(LC_ALL, "Russian"); // Устанавливаем русскую локаль
    Queue<std::string> stringQueue;
    try {
        stringQueue.push("Первый");
        stringQueue.push("Второй");
        stringQueue.pop();
        std::cout << "Первый элемент: " << stringQueue.front() << std::endl;
        stringQueue.pop();
        stringQueue.pop();
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
    Queue<int> intQueue;
    try {
        intQueue.front();
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
    return 0;
}
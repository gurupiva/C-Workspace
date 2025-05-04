#include <iostream>
#include <deque>
#include <locale> // Для корректного отображения русских символов

template <typename T>
class Queue {
private:
    std::deque<T> elements;
public:
    void push(const T& item) {
        elements.push_back(item);
    }
    void pop() {
        if (!elements.empty()) {
            elements.pop_front();
        }
    }
    T& front() {
        return elements.front();
    }
    bool empty() const {
        return elements.empty();
    }
};

int main() {
    setlocale(LC_ALL, "Russian"); // Устанавливаем локаль для русского языка
    Queue<std::string> stringQueue;
    stringQueue.push("Первый");
    stringQueue.push("Второй");
    stringQueue.push("Третий");

    std::cout << "Содержимое строковой очереди:" << std::endl;
    while (!stringQueue.empty()) {
        std::cout << stringQueue.front() << std::endl;
        stringQueue.pop();
    }
    Queue<int> intQueue;
    intQueue.push(10);
    intQueue.push(20);
    intQueue.push(30);
    std::cout << "\nСодержимое числовой очереди:" << std::endl;
    while (!intQueue.empty()) {
        std::cout << intQueue.front() << std::endl;
        intQueue.pop();
    }
    return 0;
}
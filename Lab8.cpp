#include <iostream>
#include <string>

class Person {
private:
    std::string name;
    int age;
    std::string email;
    std::string address;

public:
    // Геттеры
    std::string getName() const {
        return name;
    }

    int getAge() const {
        return age;
    }

    std::string getEmail() const {
        return email;
    }

    std::string getAddress() const {
        return address;
    }

    // Сеттеры
    void setName(const std::string& newName) {
        if (!newName.empty()) {
            name = newName;
        }
        else {
            std::cerr << "Error: Name cannot be empty!" << std::endl;
        }
    }

    void setAge(int newAge) {
        if (newAge >= 0 && newAge <= 120) {
            age = newAge;
        }
        else {
            std::cerr << "Error: Age must be between 0 and 120!" << std::endl;
        }
    }

    void setEmail(const std::string& newEmail) {
        if (newEmail.find('@') != std::string::npos) {
            email = newEmail;
        }
        else {
            std::cerr << "Error: Invalid email format!" << std::endl;
        }
    }

    void setAddress(const std::string& newAddress) {
        if (!newAddress.empty()) {
            address = newAddress;
        }
        else {
            std::cerr << "Error: Address cannot be empty!" << std::endl;
        }
    }
    void displayInfo() const {
        std::cout << "Name: " << name
            << ", Age: " << age
            << ", Email: " << email
            << ", Address: " << address 
            << std::endl;
    }
};

int main() {
    Person person;
    person.setName("Rostislav Doolgopolov");
    person.setAge(19);
    person.setEmail("rostislav.dolgopolov@example.com");
    person.setAddress("Rostov-on-Don"); 
    std::cout << "--- Correct data ---" << std::endl;
    person.displayInfo();
    std::cout << "\n--- Invalid data ---" << std::endl;
    person.setName("");
    person.setAge(150);
    person.setEmail("invalid-email");
    person.setAddress("");
    std::cout << "\n--- Final state ---" << std::endl;
    person.displayInfo();

    return 0;
}
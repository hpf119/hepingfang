#include <iostream>
#include <string>

int main() {
    std::cout << "Hello World" << std::endl;
    std::string input;
    std::cout << "Please enter something: ";
    std::getline(std::cin, input);
    std::cout << "You entered: " << input << std::endl;
    return 0;
}
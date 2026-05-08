#include <iostream>
#include <string>

int main() {
    // 输出欢迎信息
    std::cout << "Hello World" << std::endl;

    // 读取用户输入并显示
    std::string input;
    std::cout << "Please enter something: ";
    std::getline(std::cin, input);
    std::cout << "You entered: " << input << std::endl;
    return 0;
}
#ifndef TOOLKIT_H
#define TOOLKIT_H

#include <string>

class Toolkit {
public:
    static void info(std::string message);
    static void warning(std::string message);
    static void error(std::string message);
    static void errorDetail(std::string message);
    static void success(std::string message);
    static void debug(std::string message);
};

void Toolkit::info(std::string message) {
    std::cout << "\033[0;37m[Toolkit]\033[0m ";
    std::cout << "\033[0;36m" + message + "\033[0m\n";
}

void Toolkit::warning(std::string message) {
    std::cout << "\033[0;37m[Toolkit]\033[0m ";
    std::cout << "\033[0;33m" + message + "\033[0m\n";
}

void Toolkit::error(std::string message) {
    std::cout << "\033[0;37m[Toolkit]\033[0m ";
    std::cout << "\033[1;37;41m" + message + "\033[0m\n";
}

void Toolkit::errorDetail(std::string message) {
    std::cout << "\033[0;37m[Toolkit]\033[0m ";
    std::cout << "\033[0;31m" + message + "\033[0m\n";
}

void Toolkit::success(std::string message) {
    std::cout << "\033[0;37m[Toolkit]\033[0m ";
    std::cout << "\033[1;37;42m" + message + "\033[0m\n";
}

void Toolkit::debug (std::string message) {
    std::cout << "\033[0;37m[ . . . ]\033[0m ";
    std::cout << message;
}

#endif

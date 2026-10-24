#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <algorithm>
#include <functional>
#include "calculator.h"

//Перелік підтримуваних типів аргументів
enum class OptionType {
    HELP,         //-h, --help
    VERSION,      //-v, --version
    LIST,         //-l, --list 
    VALUE,        //-V<val>, --value=<val>
    LIST_VALUES   //-L<list>, --list=<list>
};

//Структура для збереження парсених даних аргументу
struct ParsedOption {
    OptionType type;
    int single_value = 0;
    std::vector<int> number_list;
};

//Фіктивні обробники (Dummy Handlers)
void handleHelp(const ParsedOption&) {
    std::cout << "Arg: Help" << std::endl;
}

void handleVersion(const ParsedOption&) {
    std::cout << "Arg: Version" << std::endl;
}

void handleList(const ParsedOption&) {
    std::cout << "Arg: List" << std::endl;
}

void handleValue(const ParsedOption& opt) {
    std::cout << "Arg: Value (" << opt.single_value << ")" << std::endl;
}

void handleListValues(const ParsedOption& opt) {
    std::cout << "Arg: List Values (";
    for (size_t i = 0; i < opt.number_list.size(); ++i) {
        std::cout << opt.number_list[i] << (i + 1 < opt.number_list.size() ? ", " : "");
    }
    std::cout << ")" << std::endl;
}

//Допоміжна функція для розбору рядка чисел через кому "1,2,3,4"
std::vector<int> parseCommaSeparatedInts(const std::string& str) {
    std::vector<int> result;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, ',')) {
        if (!token.empty()) {
            try {
                result.push_back(std::stoi(token));
            } catch (...) {
                std::cerr << "[Warning] Некоректне число у списку: '" << token << "'\n";
            }
        }
    }
    return result;
}

//Парсер параметрів командного рядка
void parseAndProcessArguments(int argc, char* argv[]) {
    std::set<OptionType> seenOptions;         //Відстеження унікальності
    std::vector<ParsedOption> uniqueOptions;  //Збереження порядку появи

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        //Обробка довгих ключів (--key або --key=value)
        if (arg.rfind("--", 0) == 0) {
            size_t eqPos = arg.find('=');
            if (eqPos != std::string::npos) {
                //Формат із значенням --value=1000 або --list=1,2,3,4
                std::string key = arg.substr(2, eqPos - 2);
                std::string val = arg.substr(eqPos + 1);

                if (key == "value") {
                    if (seenOptions.find(OptionType::VALUE) == seenOptions.end()) {
                        try {
                            int v = std::stoi(val);
                            seenOptions.insert(OptionType::VALUE);
                            uniqueOptions.push_back({OptionType::VALUE, v, {}});
                        } catch (...) {
                            std::cerr << "[Warning] Некоректне значення для '--value': " << val << "\n";
                        }
                    }
                } else if (key == "list") {
                    if (seenOptions.find(OptionType::LIST_VALUES) == seenOptions.end()) {
                        seenOptions.insert(OptionType::LIST_VALUES);
                        uniqueOptions.push_back({OptionType::LIST_VALUES, 0, parseCommaSeparatedInts(val)});
                    }
                } else {
                    std::cerr << "[Warning] Невідомий параметр з аргументом: '--" << key << "'\n";
                }
            } else {
                //Формат прапорця має бути: --help, --version, --list
                std::string key = arg.substr(2);
                OptionType opt;
                bool valid = true;

                if (key == "help") opt = OptionType::HELP;
                else if (key == "version") opt = OptionType::VERSION;
                else if (key == "list") opt = OptionType::LIST;
                else {
                    std::cerr << "[Warning] Невідомий параметр: '--" << key << "'\n";
                    valid = false;
                }

                if (valid && seenOptions.find(opt) == seenOptions.end()) {
                    seenOptions.insert(opt);
                    uniqueOptions.push_back({opt, 0, {}});
                }
            }
        } 
        
        //Обробка коротких ключів (-h, -lh, -V100, -L1,2,3)
        else if (arg.rfind("-", 0) == 0 && arg.length() > 1) {
            //Ключ -V з числовим значенням (наприклад -V100)
            if (arg[1] == 'V') {
                std::string val = arg.substr(2);
                if (!val.empty()) {
                    if (seenOptions.find(OptionType::VALUE) == seenOptions.end()) {
                        try {
                            int v = std::stoi(val);
                            seenOptions.insert(OptionType::VALUE);
                            uniqueOptions.push_back({OptionType::VALUE, v, {}});
                        } catch (...) {
                            std::cerr << "[Warning] Некоректне значення для '-V': " << val << "\n";
                        }
                    }
                } else {
                    std::cerr << "[Warning] Ключ '-V' вимагає числового значення (наприклад: -V100)\n";
                }
            } 
            //Ключ -L зі списком значень (такі як -L1,2,3,4)
            else if (arg[1] == 'L') {
                std::string val = arg.substr(2);
                if (!val.empty()) {
                    if (seenOptions.find(OptionType::LIST_VALUES) == seenOptions.end()) {
                        seenOptions.insert(OptionType::LIST_VALUES);
                        uniqueOptions.push_back({OptionType::LIST_VALUES, 0, parseCommaSeparatedInts(val)});
                    }
                } else {
                    std::cerr << "[Warning] Ключ '-L' вимагає списку значений (наприклад: -L1,2,3)\n";
                }
            } 
            //Комбіновані короткі прапорці (наприклад: -lh або -v)
            else {
                for (size_t j = 1; j < arg.length(); ++j) {
                    char c = arg[j];
                    OptionType opt;
                    bool valid = true;

                    if (c == 'h') opt = OptionType::HELP;
                    else if (c == 'v') opt = OptionType::VERSION;
                    else if (c == 'l') opt = OptionType::LIST;
                    else {
                        std::cerr << "[Warning] Невідомий короткий прапорець: '-" << c << "'\n";
                        valid = false;
                    }

                    if (valid && seenOptions.find(opt) == seenOptions.end()) {
                        seenOptions.insert(opt);
                        uniqueOptions.push_back({opt, 0, {}});
                    }
                }
            }
        } 
        else {
            std::cerr << "[Warning] Невідомий аргумент: '" << arg << "'\n";
        }
    }

    //Виклики обробників тільки для унікальних ключів
    for (const auto& opt : uniqueOptions) {
        switch (opt.type) {
            case OptionType::HELP:        handleHelp(opt); break;
            case OptionType::VERSION:     handleVersion(opt); break;
            case OptionType::LIST:        handleList(opt); break;
            case OptionType::VALUE:       handleValue(opt); break;
            case OptionType::LIST_VALUES: handleListValues(opt); break;
        }
    }
}

//Демонстрація роботи калькулятора
int main(int argc, char* argv[]) {
    //Розбір аргументів командного рядка
    parseAndProcessArguments(argc, argv);

    //Вивід результатів роботи калькулятора
    std::cout << "----------------\nCalculator Output" << std::endl;
    Calculator calc;
    std::cout << "Add (5, 3): " << calc.Add(5, 3) << std::endl;
    std::cout << "Sub (5, 3): " << calc.Sub(5, 3) << std::endl;

    return 0;
}

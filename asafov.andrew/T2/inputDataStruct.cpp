#include "datastruct.h"
#include <cctype>
#include <string>

namespace {
    void skipSpaces(std::string::const_iterator& it, const std::string::const_iterator& end) {
        while (it != end && *it == ' ') ++it;
    }
}

std::istream& asafov::operator>>(std::istream& is, DataStruct& data) {
    // Читаем посимвольно пока не найдем '\n'
    std::string input;
    char ch;
    while (is.get(ch) && ch != '\n') {
        input += ch;
    }

    auto it = input.begin();
    const auto end = input.end();
    
    unsigned long long key1 = 0;
    std::complex<double> key2 = {0.0, 0.0};
    std::string key3;
    bool has_key1 = false, has_key2 = false, has_key3 = false;

    while (it != end) {
        // Ищем ключ
        if (*it == ':') {
            ++it;
            skipSpaces(it, end);
            
            // Проверяем какой ключ
            if (it + 3 <= end && std::string(it, it+4) == "key1") {
                it += 4;
                skipSpaces(it, end);
                
                // Парсим 0b101
                if (it + 1 <= end && *it == '0' && *(it+1) == 'b') {
                    it += 2;
                    unsigned long long num = 0;
                    while (it != end && (*it == '0' || *it == '1')) {
                        num = (num << 1) | (*it == '1');
                        ++it;
                    }
                    key1 = num;
                    has_key1 = true;
                }
            }
            else if (it + 3 <= end && std::string(it, it+4) == "key2") {
                it += 4;
                skipSpaces(it, end);
                
                // Парсим #c(1.0 2.0)
                if (it + 2 <= end && *it == '#' && *(it+1) == 'c' && *(it+2) == '(') {
                    it += 3;
                    std::string real_str, imag_str;
                    while (it != end && *it != ' ' && *it != ')') {
                        real_str += *it;
                        ++it;
                    }
                    
                    if (it != end && *it == ' ') {
                        ++it;
                        while (it != end && *it != ')') {
                            imag_str += *it;
                            ++it;
                        }
                    }
                    
                    if (it != end && *it == ')') {
                        ++it;
                        try {
                            double real = std::stod(real_str);
                            double imag = imag_str.empty() ? 0.0 : std::stod(imag_str);
                            key2 = {real, imag};
                            has_key2 = true;
                        } catch (...) {}
                    }
                }
            }
            else if (it + 3 <= end && std::string(it, it+4) == "key3") {
                it += 4;
                skipSpaces(it, end);
                
                // Парсим "строка"
                if (it != end && *it == '"') {
                    ++it;
                    std::string str;
                    while (it != end && *it != '"') {
                        str += *it;
                        ++it;
                    }
                    if (it != end && *it == '"') {
                        ++it;
                        key3 = str;
                        has_key3 = true;
                    }
                }
            }
        }
        else {
            ++it;
        }
    }

    if (has_key1 && has_key2 && has_key3) {
        data.key1 = key1;
        data.key2 = key2;
        data.key3 = key3;
    } else {
        is.setstate(std::ios::failbit);
    }

    return is;
}

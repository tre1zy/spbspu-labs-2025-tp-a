#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <bitset>

struct DataStruct {
    unsigned long long key1; 
    unsigned long long key2; 
    std::string key3;

    
    friend std::istream& operator>>(std::istream& is, DataStruct& ds) {
        std::string line;
        if (!std::getline(is, line, ')')) { 
            return is; 
        }

        size_t start = line.find("(:");
        if (start == std::string::npos) {
            is.setstate(std::ios::failbit); 
            return is;
        }

        
        size_t pos = line.find("key1 ");
        if (pos != std::string::npos) {
            pos += 5; 
            ds.key1 = std::stoull(line.substr(pos, line.find("ull", pos) - pos));
        }

        pos = line.find("key2 ");
        if (pos != std::string::npos) {
            pos += 5; 
            size_t end = line.find(':', pos);
            std::string binaryStr = line.substr(pos, end - pos);
            ds.key2 = std::stoull(binaryStr, nullptr, 2); 
        }

        pos = line.find("key3\"");
        if (pos != std::string::npos) {
            pos += 5; 
            size_t end = line.find('"', pos);
            ds.key3 = line.substr(pos, end - pos);
        }

        return is;
    }

    
    friend std::ostream& operator<<(std::ostream& os, const DataStruct& ds) {
        os << "(:key1 " << ds.key1 << "ull:key2 0b"
            << std::bitset<64>(ds.key2).to_string() << ":key3 \"" << ds.key3 << "\":)";
        return os;
    }
};


bool compareDataStruct(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    if (a.key2 != b.key2) return a.key2 < b.key2;
    return a.key3.size() < b.key3.size();
}

#endif 

#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <string>

namespace martynov {

    struct DataStruct {
        unsigned long long key1;
        unsigned long long key2;
        std::string key3;
    };

    bool compare(const DataStruct& a, const DataStruct& b);

}  

#endif  
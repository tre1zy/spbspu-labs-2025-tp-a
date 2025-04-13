#include "DataStruct.h"
#include <vector>
#include <iterator>
#include <algorithm>

int main()
{
    std::vector<nspace::DataStruct> data;

    while (!std::cin.eof()) 
    {
        nspace::DataStruct temp;
        if (std::cin >> temp)
        {
            data.push_back(temp);
        }
        else 
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::sort(data.begin(), data.end());

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<nspace::DataStruct>(std::cout, "\n")
    );

    return 0;
}

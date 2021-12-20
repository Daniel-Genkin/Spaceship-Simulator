#include "Util/StringOperations.h"

// tokenizes a string
vector<string> StringOperations::split(string input, char delimiter)
{
    vector<string> result;

    if (input.length() == 0)
    {
        return result;
    }
    unsigned short startIndex = 0;
    unsigned short endIndex = 1;
    while (endIndex < input.length())
    {
        if (input[endIndex] == delimiter)
        {
            if (startIndex == endIndex)
            {
                startIndex++;
                endIndex++;
                result.push_back("");
                continue;
            }
            result.push_back(input.substr(startIndex, endIndex - startIndex));
            startIndex = endIndex + 1;
        }
        endIndex++;
    }
    result.push_back(input.substr(startIndex)); // add last element
    return result;
}
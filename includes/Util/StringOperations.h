#ifndef STRINGOPERATIONS_H
#define STRINGOPERATIONS_H

#include <vector>
#include <string>

using namespace std;

class StringOperations
{
public:
    static vector<string> split(string input, char delimiter);
};

#endif
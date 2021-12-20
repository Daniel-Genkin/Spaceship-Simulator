#ifndef OBJECTIVEMESSAGE_H
#define OBJECTIVEMESSAGE_H

#include <string>

using namespace std;

enum class Status
{
    NO_PROGRESS,
    FINISHED,
    NEEDS_HINT
};

class ObjectiveMessage
{
public:
    Status status;
    string msg;
};

#endif /* OBJECTIVEMESSAGE_H */

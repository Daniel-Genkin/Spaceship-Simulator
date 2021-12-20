#ifndef POINT2_H
#define POINT2_H

class Point2
{
public:
    float x, y;
    Point2(float x, float y)
    {
        this->x = x;
        this->y = y;
    }
    Point2()
    {
        this->x = 0;
        this->y = 0;
    }
    ~Point2() {}
};

#endif
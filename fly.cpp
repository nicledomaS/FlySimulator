#include <iostream>
#include <cstdlib>
#include <ctime>
#include "fly.h"

using namespace std;

static const vector<Direction> directions = {{-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}};

Fly::Fly(const unsigned short x, const unsigned short y, const size_t stup)
    :   xPos(x),
        yPos(y),
        stupidity(stup)
{
}

void Fly::goTo(Area &area)
{
    Direction dir = directions[rand() % 8];

    unsigned short x = xPos;
    if((x == 0 && dir.dX < 0) || (x == area.size() - 1 && dir.dX > 0))
        x += (~dir.dX) + 1;
    else
        x += dir.dX;

    unsigned short y = yPos;
    if((y == 0 && dir.dY < 0) || (y == area.size()  - 1 && dir.dY > 0))
        y += (~dir.dY) + 1;
    else
        y += dir.dY;

    if(area.goTo(xPos, yPos, x, y))
    {
        xPos = x;
        yPos = y;
        ++telemetry.distance;
    }
    if(++telemetry.age == area.size())
    {
        telemetry.state = Telemetry::Died;
        area.die(xPos, yPos);
    }
    telemetry.speed = telemetry.distance / telemetry.age;
}


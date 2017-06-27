#include "area.h"

Area::Area(const size_t maxPos, const size_t count, QObject * parent)
    :   QObject(parent),
      fieldSize(maxPos),
      flyCapacity(count)
{

}

Area::~Area()
{

}

bool Area::goToCell(const unsigned short curX, const unsigned short curY,
                    const unsigned short newX, const unsigned short newY)
{
    std::unique_lock<std::mutex> lock(mtx);
    if(!goIn(createKey(newX, newY)))
        return false;
    goOut(createKey(curX, curY));
    emit updated();
    return true;
}

bool Area::flySat(const unsigned short x, const unsigned short y)
{
    std::unique_lock<std::mutex> lock(mtx);
    return goIn(createKey(x, y));
}

void Area::flyDied(const unsigned short x, const unsigned short y)
{
    std::unique_lock<std::mutex> lock(mtx);
    unsigned long position = createKey(x, y);
    auto it = cells.find(position);
    if(it == cells.end())
        cells[position] =  Cell(1, 1);
    else
        ++(*it).second.diedFlies;
}

Cell Area::getData(const unsigned short x, const unsigned short y) const
{
    auto it = cells.find(createKey(x, y));
    if(it == cells.end())
        return Cell();
    return it->second;
}

bool Area::goIn(const unsigned long position)
{
    auto it = cells.find(position);
    if(it == cells.end())
    {
        cells[position] = Cell(1, 0);
        return true;
    }
    else if((*it).second.flies < flyCapacity)
    {
        ++(*it).second.flies;
        return true;
    }
    return false;
}

void Area::goOut(const unsigned long position)
{
    auto it =cells.find(position);
    if(it == cells.end())
        return;
    --(*it).second.flies;
}

unsigned long Area::createKey(const unsigned short x, const unsigned short y) const
{
    unsigned long key = x;
    key <<= 16;
    key |= y;
    return key;
}

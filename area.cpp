#include "area.h"

static unsigned long createKey(const unsigned short x, const unsigned short y)
{
    unsigned long key = x;
    key <<= 16;
    key |= y;
    return key;
}

Area::Area(size_t maxPos, size_t count, QObject * parent)
    :   QObject(parent),
      fieldSize(maxPos),
      flyCapacity(count)
{

}

Area::~Area()
{

}

bool Area::goTo(unsigned short curX, unsigned short curY,
                    unsigned short newX, unsigned short newY)
{
    std::unique_lock<std::mutex> lock(mtx);
    if(!goIn(createKey(newX, newY)))
        return false;
    goOut(createKey(curX, curY));
    emit updated();
    return true;
}

bool Area::landing(unsigned short x, unsigned short y)
{
    std::unique_lock<std::mutex> lock(mtx);
    return goIn(createKey(x, y));
}

void Area::die(unsigned short x, unsigned short y)
{
    std::unique_lock<std::mutex> lock(mtx);
    unsigned long position = createKey(x, y);
    auto it = cells.find(position);
    if(it == cells.end())
        cells[position] =  Cell(1, 1);
    else
    {
        Cell & cell = (*it).second;
        ++cell.died;
    }
}

Cell Area::population(const unsigned short x, const unsigned short y) const
{
    auto it = cells.find(createKey(x, y));
    if(it == cells.end())
        return Cell();
    return it->second;
}

bool Area::goIn(unsigned long position)
{
    auto it = cells.find(position);
    if(it == cells.end())
    {
        cells[position] = Cell(1, 0);
        return true;
    }

    Cell & cell = (*it).second;
    if(cell.all < flyCapacity)
    {
        ++cell.all;
        return true;
    }
    return false;
}

void Area::goOut(unsigned long position)
{
    auto it =cells.find(position);
    if(it == cells.end())
        return;

    Cell & cell = (*it).second;
    --cell.all;
}

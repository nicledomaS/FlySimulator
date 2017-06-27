#ifndef AREA_H
#define AREA_H
#include <map>
#include <mutex>
#include <QObject>

struct Cell
{
    unsigned int flies;
    unsigned int diedFlies;
    Cell(int cf = 0, int cd = 0)
        :flies(cf),diedFlies(cd){}

public:
    unsigned int getCountFlies() const;
    void setCountFlies(unsigned int value);
};

class Area : public QObject
{
    Q_OBJECT
public:
    Area(const size_t maxPos, const size_t count, QObject *parent = 0);
    virtual ~Area();

    bool goToCell(const unsigned short curX, const unsigned short curY,
                  const unsigned short newX, const unsigned short newY);
    bool flySat(const unsigned short x, const unsigned short y);
    void flyDied(const unsigned short x, const unsigned short y);
    Cell getData(const unsigned short x, const unsigned short y) const;
    void setParams(size_t fSize, size_t fCap) { fieldSize = fSize; flyCapacity = fCap; }
    size_t size() const {return fieldSize;}
    size_t count() const {return flyCapacity;}
    void clear() {cells.clear();}
signals:
    void updated();

private:

    bool goIn(const unsigned long position);
    void goOut(const unsigned long position);
    unsigned long createKey(const unsigned short x, const unsigned short y) const;

    size_t fieldSize;
    size_t flyCapacity;
    std::map<unsigned long, Cell> cells;
    std::mutex mtx;
};



#endif // AREA_H

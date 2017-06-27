#ifndef AREA_H
#define AREA_H
#include <map>
#include <mutex>
#include <QObject>

struct Cell
{
    unsigned int all;
    unsigned int died;
    Cell(int cf = 0, int cd = 0)
        :all(cf),died(cd) {}
};

class Area : public QObject
{
    Q_OBJECT
public:
    Area(size_t maxPos, size_t count, QObject *parent = 0);
    virtual ~Area();
    bool goTo(unsigned short curX, unsigned short curY,
                  unsigned short newX, unsigned short newY);
    bool landing(unsigned short x, unsigned short y);
    void die(unsigned short x, unsigned short y);
    Cell population(unsigned short x, unsigned short y) const;
    void setParams(size_t fSize, size_t fCap) { fieldSize = fSize; flyCapacity = fCap; }
    size_t size() const { return fieldSize; }
    void clear() { cells.clear(); }
signals:
    void updated();
private:
    bool goIn(unsigned long position);
    void goOut(unsigned long position);

    size_t fieldSize;
    size_t flyCapacity;
    std::map<unsigned long, Cell> cells;
    std::mutex mtx;
};

#endif // AREA_H

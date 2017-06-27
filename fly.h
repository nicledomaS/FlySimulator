#ifndef FLY_H
#define FLY_H
#include <memory>
#include <vector>
#include <string>
#include "area.h"

struct Telemetry
{
    enum { Died, Live };
    Telemetry()
        : age(0), distance(0), speed(0), state(Live) {}

    std::string toString() const
    {
        std::string str;
        str.append("Lifetime: ")
            .append(std::to_string(float(age)/1000.0))
            .append(" sec, Distance: ")
            .append(std::to_string(distance))
            .append(" cells, Speed: ")
            .append(std::to_string(speed))
            .append(" cell/sec")
           .append(", Status: ")
           .append((state == Live ? "Live" : "Died"));
        return str;
    }

    size_t age;
    size_t distance;
    float speed;
    unsigned char state;
};

class Fly
{
public:
    typedef std::shared_ptr<Fly> Ptr_t;
    Fly(const unsigned short x, const unsigned short y, const size_t stup);
    size_t think() const { return stupidity; }
    void goTo(Area& area);
    const Telemetry &getTelemetry() const { return telemetry; }
    unsigned char getState() const { return telemetry.state; }
    void printTelemetry() const;
private:
    Fly(const Fly&);
    Fly& operator = (const Fly&);

    unsigned short xPos;
    unsigned short yPos;
    const size_t stupidity;
    Telemetry telemetry;
    std::mt19937 rgen;
};

struct Direction
{
    char dX;
    char dY;
};

#endif // FLY_H

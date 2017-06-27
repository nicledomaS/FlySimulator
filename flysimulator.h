#ifndef FLYSIMULATOR_H
#define FLYSIMULATOR_H
#include <thread>
#include <vector>
#include "area.h"
#include "fly.h"

class FlySimulator
{
public:
    FlySimulator(const size_t maxPos = 20, const size_t count = 15, const size_t stup = 200);
    ~FlySimulator();

    void setParams(size_t filedSize, size_t capacity, size_t stup);
    void start();
    void stop();
    void addFly();
    const Area& getArea() const;
    const std::vector<Fly::Ptr_t> getFlies() const;

private:
    Area area;
    size_t stupidity;
    bool stopFlag;
    std::vector<Fly::Ptr_t> flies;
    std::vector<std::thread> threads;
};

#endif // FLYSIMULATOR_H

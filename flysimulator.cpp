#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "flysimulator.h"

using namespace std;

FlySimulator::FlySimulator(const size_t maxPos, const size_t count, const size_t stup)
    :   area(maxPos, count),
        stupidity(stup),
        stopFlag(false)
{
    srand(time(NULL));
}

FlySimulator::~FlySimulator()
{
    stop();
}

void FlySimulator::setParams(size_t fieldSize, size_t capacity, size_t stup)
{
    area.setParams(fieldSize,capacity);
    stupidity = stup;
}

void FlySimulator::start()
{
    flies.clear();
    area.clear();
    stopFlag = false;
    for(int i = 0; i < 3; ++i)
        addFly();
}

void FlySimulator::stop()
{
    stopFlag = true;
    for_each(threads.begin(), threads.end(), [](thread& thr)
    {
        if(thr.joinable())
            thr.join();
    });
    threads.clear();
}

void FlySimulator::addFly()
{
    unsigned short x = rand() % area.size();
    unsigned short y = rand() % area.size();

    if(!area.flySat(x, y))
        return;

    auto flyProcess = [this](Area& rArea, Fly::Ptr_t pFly)
    {
        if(!pFly)
            return;

        while(pFly->getState())
        {
            if(stopFlag)
                return;
            this_thread::sleep_for(chrono::milliseconds(pFly->think()));
            pFly->goTo(rArea);
        } 
    };

    Fly::Ptr_t pFly = make_shared<Fly>(x, y, stupidity);
    flies.push_back(pFly);
    threads.push_back(thread(flyProcess, ref(area), pFly));
}

const Area &FlySimulator::getArea() const
{
    return area;
}

const std::vector<Fly::Ptr_t> FlySimulator::getFlies() const
{
    return flies;
}

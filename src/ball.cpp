
#include "ball.h"
#include <cmath>

std::mutex Ball::_mutex;
std::condition_variable Ball::_condition;
int Ball::_cmd = 0;
int Ball::_last_id = 0;

void Ball::addSpeed(const Ball &value, double time)
{
    double xDist = value._x - _x;
    double yDist = value._y - _y;
    double r = sqrt(pow(xDist, 2) + pow(yDist, 2));
    if (r < 1)
        return;

    double f = 1/r - 1/(pow(r, 2));
    _speed_x += (xDist / r) * f * time;
    _speed_y += (yDist / r) * f * time;
}

void Ball::run(std::vector<Ball> &balls, std::function<void()> callback)
{
    for (;;)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        for (;;)
        {
            if (_cmd < 0)
                return;
            if (_cmd > 0)
                break;
            _condition.wait(lock);
        }

        _cmd = 0;

        move(balls, 1);

        callback();
    }
}

void Ball::move(std::vector<Ball> &balls, double time)
{
    for (auto &a: balls)
    {
        for (auto &b: balls)
        {
            if (a._id == b._id)
                continue;
            a.addSpeed(b, time);
        }
    }

    for (auto &a: balls)
    {
        a._x += a._speed_x * time;
        a._y += a._speed_y * time;
    }
}

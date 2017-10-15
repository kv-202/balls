#ifndef BALL_H
#define BALL_H

#include <memory>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <utility>

class Ball
{
public:
    void addSpeed(const Ball &value, double time);

    int _id;
    double _x;
    double _y;
    double _speed_x;
    double _speed_y;

    static void run(std::vector<Ball> &balls, std::function<void()> callback);
    static void move(std::vector<Ball> &balls, double time);

    static std::mutex _mutex;
    static std::condition_variable _condition;
    static int _cmd;
    static int _last_id;
};

#endif // BALL_H

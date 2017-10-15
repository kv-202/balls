
#include "ball.h"
#include "ballgraphicsview.h"
#include <QApplication>
#include <thread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::vector<Ball> balls;

    BallGraphicsView v(balls);

    std::thread t(&Ball::run, std::ref(balls), [&v] () { v.updateItemsSignal(); });

    v.show();

    int ret = a.exec();

    Ball::_mutex.lock();
    Ball::_cmd = -1;
    Ball::_mutex.unlock();
    Ball::_condition.notify_all();

    t.join();

    return ret;
}

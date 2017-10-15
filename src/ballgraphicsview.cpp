
#include "ballgraphicsview.h"
#include <QMouseEvent>
#include <QDragMoveEvent>
#include <QDrag>
#include <QMimeData>
#include <thread>

static const int _TIME_INT = 100;
static const int _DATA_ID_KEY = 1;

void BallGraphicsView::updateItemsSlot()
{
    auto view_items = items();
    for (auto view_item: view_items)
    {
        auto view_ball = dynamic_cast<EllipseAnimation*>(view_item);
        if (!view_ball)
            continue;

        int id = view_ball->data(_DATA_ID_KEY).toInt();
        auto it = std::find_if(_balls.begin(), _balls.end(), [&id] (const auto& a) { return a._id == id; });
        if (it == _balls.end())
            continue;

        view_ball->moveToPos(QPointF(it->_x, it->_y), _TIME_INT);
    }
}

BallGraphicsView::BallGraphicsView(std::vector<Ball> &balls):
    _balls(balls),
    _drag_item(nullptr),
    _timer(0)
{
    setAcceptDrops(true);

    _scene.setSceneRect(QRect(0, 0, 500, 500));
    setScene(&_scene);

    move(100, 100);
    resize(600, 600);

    connect(this, &BallGraphicsView::updateItemsSignal, this, &BallGraphicsView::updateItemsSlot, Qt::BlockingQueuedConnection);

    _timer = startTimer(_TIME_INT);

    appendRandomItems(10);
}

BallGraphicsView::~BallGraphicsView()
{
    if (_timer)
        killTimer(_timer);
}

void BallGraphicsView::mousePressEvent(QMouseEvent *e)
{
    auto pos = mapToScene(e->pos());
    auto item = dynamic_cast<EllipseAnimation*>(_scene.itemAt(pos, transform()));
    if (item)
    {
        int id = item->data(_DATA_ID_KEY).toInt();
        if (!(e->buttons() & Qt::MouseButton::LeftButton))
        {
            _tasks.push({ Task::Del, id, QPointF() });
            _scene.removeItem(item);
        }
        else
        {
            QDrag *drag = new QDrag(this);
            QMimeData *data = new QMimeData();
            data->setText(QString::number(id));
            drag->setMimeData(data);
            _drag_item = item;
            drag->exec();
        }
    }
    else
    {
        item = new EllipseAnimation();
        item->setPos(pos);
        item->setData(_DATA_ID_KEY, ++Ball::_last_id);
        _scene.addItem(item);
        _tasks.push({ Task::Add, item->data(_DATA_ID_KEY).toInt(), pos });
    }

    QGraphicsView::mousePressEvent(e);
}

void BallGraphicsView::dragMoveEvent(QDragMoveEvent *e)
{
    if (e->source() == this)
        e->accept();
    else
        e->ignore();
}

void BallGraphicsView::dropEvent(QDropEvent *e)
{
    QPointF pos = mapToScene(e->pos());
    _drag_item->setPos(pos);
    _tasks.push({ Task::Move, _drag_item->data(_DATA_ID_KEY).toInt(), pos });
    _drag_item = nullptr;
}

void BallGraphicsView::timerEvent(QTimerEvent */*e*/)
{
    if (!Ball::_mutex.try_lock())
        return;

    while (_tasks.size())
    {
        auto task = _tasks.front();
        _tasks.pop();
        int id = task._id;
        switch (task._action)
        {
            case Task::Add:
                _balls.push_back( { id, task._pt.x(), task._pt.y(), 0, 0 } );
            break;
            case Task::Del:
                _balls.erase(std::remove_if(_balls.begin(), _balls.end(), [&id] (auto a) { return a._id == id; }), _balls.end());
            break;
            case Task::Move:
            {
                auto it = std::find_if(_balls.begin(), _balls.end(), [&id] (auto a) { return a._id == id; });
                if (it != _balls.end())
                    *it = { id, task._pt.x(), task._pt.y(), 0, 0 };
            }
            break;
        }
    }

    Ball::_cmd = 1;
    Ball::_mutex.unlock();
    Ball::_condition.notify_all();
}

void BallGraphicsView::appendRandomItems(int count)
{
    std::srand(unsigned(std::time(0)));

    auto r = _scene.sceneRect();

    for (int i = 0; i < count; ++i)
    {
        auto x = (std::rand() % static_cast<int>(r.width()) + r.left());
        auto y = (std::rand() % static_cast<int>(r.height()) + r.top());
        QPointF pos(x, y);

        auto item = new EllipseAnimation();
        item->setPos(pos);
        item->setData(_DATA_ID_KEY, ++Ball::_last_id);
        _scene.addItem(item);
        _tasks.push({ Task::Add, item->data(_DATA_ID_KEY).toInt(), pos });
    }

}

#ifndef BALLGRAPHICSVIEW_H
#define BALLGRAPHICSVIEW_H

#include "ball.h"
#include "ellipseanimation.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <queue>

class BallGraphicsView : public QGraphicsView
{
    Q_OBJECT
signals:
    void updateItemsSignal();

private slots:
    void updateItemsSlot();

public:
    BallGraphicsView(std::vector<Ball> &balls);
    ~BallGraphicsView();

    void updateItems();

protected:  
    virtual void mousePressEvent(QMouseEvent *e) override;
    virtual void dragMoveEvent(QDragMoveEvent *e) override;
    virtual void dropEvent(QDropEvent *e) override;
    virtual void timerEvent(QTimerEvent *e) override;

private:
    void appendRandomItems(int count);

private:
    QGraphicsScene _scene;
    std::vector<Ball> &_balls;
    EllipseAnimation* _drag_item;
    int _timer;

    struct Task
    {
        enum Action
        {
            Add,
            Move,
            Del,
        };

        Action _action;
        int _id;
        QPointF _pt;
    };

    std::queue<Task> _tasks;
};

#endif // BALLGRAPHICSVIEW_H

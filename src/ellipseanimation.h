#ifndef ELLIPSEANIMATION_H
#define ELLIPSEANIMATION_H

#include <QObject>
#include <QGraphicsEllipseItem>
#include <QPropertyAnimation>

class EllipseAnimation : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
    Q_PROPERTY(QRectF rect READ rect WRITE setRect)
public:
    EllipseAnimation();

    void setPos(const QPointF &pt);
    void moveToPos(const QPointF &pt, int time);

private:
    QPropertyAnimation _animation;
};

#endif // ELLIPSEANIMATION_H

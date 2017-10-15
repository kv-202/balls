
#include "ellipseanimation.h"

static const QSize _ELLIPSE_WIDTH = QSize(20, 20);

EllipseAnimation::EllipseAnimation():
    _animation(this, "rect")
{
}

static QRectF getRect(const QPointF &pt)
{
    QPointF lt(pt.x() - (_ELLIPSE_WIDTH.width() / 2), pt.y() - (_ELLIPSE_WIDTH.height() / 2));
    return QRectF(lt, _ELLIPSE_WIDTH);
}

void EllipseAnimation::setPos(const QPointF &pt)
{
    _animation.stop();
    setRect(getRect(pt));
}

void EllipseAnimation::moveToPos(const QPointF &pt, int time)
{
    _animation.setEndValue(getRect(pt));
    _animation.setDuration(time);
    _animation.setEasingCurve(QEasingCurve::Linear);
    _animation.start();
}

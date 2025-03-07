#include "arrow.h"

#include <QPainter>
#include <QtMath>
#include <QtAssert>


// Arrow head is a equilateral triangle.
static constexpr qreal ARROW_HEAD_HEIGHT_LENGTH { 10.0 };

// height / sqrt(3)
static constexpr qreal ARROW_HEAD_HALF_SIDE { ARROW_HEAD_HEIGHT_LENGTH / 1.73205080757 };


Arrow::Arrow(const QPointF& startPoint, qreal length, qreal angle, QGraphicsItem* parent)
    : QGraphicsLineItem(parent)
    , m_startPoint(startPoint)
    , m_length(length)
    , m_angle(angle)
{
    reset();
}


Arrow::Arrow(const QPointF& startPoint, const QPointF& endPoint, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
    resetFromPoints(startPoint, endPoint);
}

Arrow* Arrow::fromVelocity(const QPointF& startPoint, const QPointF& velocity, QGraphicsItem *parent)
{
    Arrow *arrow = new Arrow(startPoint, velocity, parent);
    arrow->resetFromVelocity(startPoint, velocity);
    return arrow;
}


Arrow* Arrow::fromPoints(const QPointF& startPoint, const QPointF& endPoint, QGraphicsItem *parent)
{
    return new Arrow(startPoint, endPoint, parent);
}


void Arrow::setStartPoint(const QPointF& startPoint)
{
    m_startPoint = startPoint;
    reset();
}

void Arrow::setLength(qreal length)
{
    Q_ASSERT(length > 0);

    m_length = length;
    reset();
}

void Arrow::setAngle(qreal angle)
{
    m_angle = angle;
    reset();
}

void Arrow::reset()
{
    setLine(QLineF(m_startPoint, m_startPoint + QPointF(m_length * qCos(m_angle), m_length * qSin(m_angle))));
    qDebug() << Q_FUNC_INFO << " line points: " << line().p1() << " " << line().p2();
    //update();
}

void Arrow::resetFromPoints(const QPointF& startPoint, const QPointF& endPoint)
{
    QPointF diff = endPoint - startPoint;
    m_startPoint = startPoint;
    m_length = qHypot(diff.x(), diff.y());
    m_angle = qAtan(diff.y() / diff.x());
    qDebug() << Q_FUNC_INFO << " start and end points: " << startPoint << " " << endPoint << " " << m_length << " " << m_angle;
    reset();
}

void Arrow::resetFromVelocity(const QPointF& startPoint, const QPointF& velocity)
{
    m_startPoint = startPoint;
    m_length = qHypot(velocity.x(), velocity.y());
    m_angle = qAtan(velocity.y() / velocity.x());
    reset();
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QLineF l = line();

    painter->setPen(pen());
    painter->setBrush(QBrush(pen().color()));
    painter->setRenderHint(QPainter::Antialiasing);

    // draw line
    painter->drawLine(l);

    // construct arrowhead polygon and draw it
    //QPolygonF arrowHead;
    //arrowHead << QPointF { l.p2() + QPointF{ ARROW_HEAD_HEIGHT_LENGTH * qCos(m_angle),
    //                                        -ARROW_HEAD_HEIGHT_LENGTH * qSin(m_angle) } }

    //          << QPointF { l.p2() - QPointF{ ARROW_HEAD_HALF_SIDE * qSin(m_angle),
    //                                        -ARROW_HEAD_HALF_SIDE * qCos(m_angle) } }

    //         << QPointF { l.p2() + QPointF{ ARROW_HEAD_HALF_SIDE * qSin(m_angle),
    //                                         -ARROW_HEAD_HALF_SIDE * qCos(m_angle) } };

    //painter->drawPolygon(arrowHead);
}

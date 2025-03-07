#pragma once

#include <QGraphicsLineItem>


class Arrow : public QGraphicsLineItem
{
public:
    Arrow(const QPointF& startPoint, qreal length, qreal angle, QGraphicsItem *parent = nullptr);
    Arrow(const QPointF& startPoint, const QPointF& endPoint, QGraphicsItem *parent = nullptr);

    static Arrow* fromVelocity(const QPointF& startPoint, const QPointF& velocity, QGraphicsItem *parent = nullptr);
    static Arrow* fromPoints(const QPointF& startPoint, const QPointF& endPoint, QGraphicsItem *parent = nullptr);

public:
    void setStartPoint(const QPointF& startPoint);
    QPointF startPoint() const { return m_startPoint; }
    void setLength(qreal length);
    qreal length() const { return m_length; }
    void setAngle(qreal angle);
    qreal angle() const { return m_angle; }

    // apply current settings
    void reset();
    void resetFromPoints(const QPointF& startPoint, const QPointF& endPoint);
    void resetFromVelocity(const QPointF& startPoint, const QPointF& velocity);

public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    QPointF m_startPoint;
    qreal m_length;
    qreal m_angle;
};

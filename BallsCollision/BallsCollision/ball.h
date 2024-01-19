#pragma once

#include <QGraphicsEllipseItem>
#include <QtMath>



class Ball : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    Ball(const QPointF& startPosition, const QPointF& velocity,
         qreal radius, qreal mass, QGraphicsItem *parent = nullptr);

    Ball(qreal startPositionX, qreal startPositionY, qreal velocityX,
         qreal velocityY, qreal radius, qreal mass, QGraphicsItem *parent = nullptr);

	~Ball();

public:
    void update(const std::chrono::milliseconds& ms, Ball *another = nullptr);
    void reset(const QPointF& position, const QPointF& velocity, qreal radius, qreal mass);

    void setPosition(const QPointF& position)
    {
        setPos(position);
        emit posChanged(position);
    }

    void setPosition(qreal x, qreal y) { setPosition(QPointF { x, y }); }

    void setVelocity(const QPointF& velocity)
    {
        m_velocity = velocity;
        emit velocityChanged(velocity);
    }

    void setVelocity(qreal x, qreal y) { setVelocity( QPointF { x, y }); }

    QPointF velocity() const { return m_velocity; }
    QPointF& rvelocity() { return m_velocity; }
    qreal velocityLength() const { return qHypot(m_velocity.x(), m_velocity.y()); }
    qreal velocityAngle() const { return qAtan(m_velocity.y() / m_velocity.x()); }

    void changeVelocityLength(qreal length)
    {
        qreal coeff = qHypot(m_velocity.x(), m_velocity.y()) / length;
        setVelocity(QPointF{ m_velocity.x() / coeff, m_velocity.y() / coeff });
    }

    void changeVelocityAngle(qreal angle)
    {
        qreal length = qHypot(m_velocity.x(), m_velocity.y());
        setVelocity(QPointF { length * qCos(angle), length * qSin(angle) });
    }

    void setRadius(qreal radius)
    {
        m_radius = radius;
        setRect(0.0, 0.0, radius, radius);

        emit radiusChanged(radius);
    }

    qreal radius() const { return m_radius; }

    void setMass(qreal mass)
    {
        m_mass = mass;
        emit massChanged(mass);
    }

    qreal mass() const { return m_mass; }
    QPointF center() const { return scenePos() + QPointF { m_radius, m_radius }; }

protected:
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value) override;

signals:
    void posChanged(const QPointF& newPos);
    void velocityChanged(const QPointF& newVelocity);
    void radiusChanged(qreal newRadius);
    void massChanged(qreal newMass);

    void wallCollisionOccurred();

private:

    QPointF m_velocity;
    qreal m_radius;
    qreal m_mass;
};

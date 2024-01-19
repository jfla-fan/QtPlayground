#include "ball.h"

#include <chrono>
#include <QGraphicsScene>


Ball::Ball(const QPointF& startPosition, const QPointF& velocity, qreal radius, qreal mass, QGraphicsItem *parent)
    : QGraphicsEllipseItem(0.0, 0.0, 2 * radius, 2 * radius, parent)
    , QObject()
{
    reset(startPosition, velocity, radius, mass);
}

Ball::Ball(qreal startPositionX, qreal startPositionY, qreal velocityX,
           qreal velocityY, qreal radius, qreal mass, QGraphicsItem *parent)
    : Ball(QPointF { startPositionX, startPositionY }, QPointF { velocityX, velocityY }, radius, mass, parent)
{
}

Ball::~Ball()
{
}

void Ball::update(const std::chrono::milliseconds& ms, Ball *another)
{
    qreal dx = m_velocity.x() * ms.count() / 1000;
    qreal dy = m_velocity.y() * ms.count() / 1000;
    bool wallCollisionOccured = false;

    qDebug() << "Scene pos - " << scenePos();

    // Check for collision with left or right walls
    if (scenePos().x() + dx < 0 || scenePos().x() + 2 * m_radius + dx > scene()->width())
    {
        m_velocity.setX(-m_velocity.x()); // Reverse the x-component of velocity

        if (another != nullptr)
        {
            another->setVelocity(m_velocity.x(), another->velocity().y());
        }

        wallCollisionOccured = true;
    }

    // Check for collision with top or bottom walls
    if (scenePos().y() + dy < 0 || scenePos().y() + 2 * m_radius + dy > scene()->height())
    {
        m_velocity.setY(-m_velocity.y()); // Reverse the y-component of velocity

        if (another != nullptr)
        {
            another->setVelocity(another->velocity().x(), m_velocity.y());
        }

        wallCollisionOccured = true;
    }

    moveBy(dx, dy);
    emit posChanged(scenePos());

    if (wallCollisionOccured)
    {
        emit wallCollisionOccurred();
        emit velocityChanged(m_velocity);
    }
}

void Ball::reset(const QPointF& position, const QPointF& velocity, qreal radius, qreal mass)
{
    setPos(position);
    setVelocity(velocity);
    setRadius(radius);
    setMass(mass);
    setFlag(GraphicsItemFlag::ItemIsMovable);
}


QVariant Ball::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    if (change == ItemPositionChange)
    {
        emit posChanged(value.toPointF());
    }

    return QGraphicsItem::itemChange(change, value);
}

#include "collisionStrategy.h"

#include "ball.h"
#include "config.hpp"

#include <QObject>
#include <QGraphicsScene>


using enum BallsCollision::ECollisionStrategyType;

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief The CPerfectlyElasticCollisionStrategy class
///
/// \details Represents totally elastic collision between 2 balls.
class CPerfectlyElasticCollisionStrategy : public ICollisionStrategy
{
    Q_OBJECT
public:
    CPerfectlyElasticCollisionStrategy(const BallsCollision::BallInfo& redBallInfo,
                                       const BallsCollision::BallInfo& blueBallInfo,
                                       QGraphicsScene *scene)
        : m_redBallInfo(redBallInfo)
        , m_blueBallInfo(blueBallInfo)
        , m_scene(scene)
    {
        connect(m_redBallInfo.m_ball, &Ball::wallCollisionOccurred, [this]() { emit collisionDetected(); });
        connect(m_blueBallInfo.m_ball, &Ball::wallCollisionOccurred, [this]() { emit collisionDetected(); });
    }

    ~CPerfectlyElasticCollisionStrategy() override { }

public:
    BallsCollision::ECollisionStrategyType type() const override { return eCST_PerfectlyElastic; }

    void handleCollisions(BallsCollision *) override
    {
        m_redBallInfo.m_ball->update(Config::MILLISECONDS_PER_FRAME);
        m_blueBallInfo.m_ball->update(Config::MILLISECONDS_PER_FRAME);

        if (m_redBallInfo.m_ball->collidesWithItem(m_blueBallInfo.m_ball))
        {
                emit collisionDetected();

                // Calculate relative velocity
                //QPointF relativeVelocity = m_blueBallInfo.m_ball->velocity() - m_redBallInfo.m_ball->velocity();

                // Calculate the normal vector (normalized vector pointing from red to blue ball)
                //QPointF normal = m_blueBallInfo.m_ball->pos() - m_redBallInfo.m_ball->pos();
                //qreal distance = sqrt(normal.x() * normal.x() + normal.y() * normal.y());
                //normal /= distance;

                // Calculate the component of relative velocity along the normal vector
                //qreal relativeSpeed = relativeVelocity.x() * normal.x() + relativeVelocity.y() * normal.y();

                // Calculate the new velocities after the collision (perfectly elastic collision)
                //QPointF redNewVelocity = m_redBallInfo.m_ball->velocity() + (2 * m_blueBallInfo.m_ball->mass() / (m_redBallInfo.m_ball->mass() + m_blueBallInfo.m_ball->mass())) * relativeSpeed * normal;
                //QPointF blueNewVelocity = m_blueBallInfo.m_ball->velocity() - (2 * m_redBallInfo.m_ball->mass() / (m_redBallInfo.m_ball->mass() + m_blueBallInfo.m_ball->mass())) * relativeSpeed * normal;

                qreal totalMass = m_redBallInfo.m_ball->mass() + m_blueBallInfo.m_ball->mass();

                QPointF redNewVelocity = ((m_redBallInfo.m_ball->mass() - m_blueBallInfo.m_ball->mass()) * m_redBallInfo.m_ball->velocity() +
                                          2 * m_blueBallInfo.m_ball->mass() * m_blueBallInfo.m_ball->velocity()) / totalMass;

                QPointF blueNewVelocity = ((m_blueBallInfo.m_ball->mass() - m_redBallInfo.m_ball->mass()) * m_blueBallInfo.m_ball->velocity() +
                                           2 * m_redBallInfo.m_ball->mass() * m_redBallInfo.m_ball->velocity()) / totalMass;

                // Update the velocities of the balls
                m_redBallInfo.m_ball->setVelocity(redNewVelocity);
                m_blueBallInfo.m_ball->setVelocity(blueNewVelocity);
        }
    }

private:
    BallsCollision::BallInfo m_redBallInfo, m_blueBallInfo;
    QGraphicsScene *m_scene;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief The CPerfectlyInElasticCollisionStrategy class
///
/// \details Represents totally in-elastic collision between 2 balls.
class CPerfectlyInElasticCollisionStrategy : public ICollisionStrategy
{
    Q_OBJECT
public:
    CPerfectlyInElasticCollisionStrategy(const BallsCollision::BallInfo& redBallInfo,
                                         const BallsCollision::BallInfo& blueBallInfo,
                                         QGraphicsScene *scene)
        : m_redBallInfo(redBallInfo)
        , m_blueBallInfo(blueBallInfo)
        , m_scene(scene)
        , m_collided(false)
    {
        connect(m_redBallInfo.m_ball, &Ball::wallCollisionOccurred, [this]() { emit collisionDetected(); });
        connect(m_blueBallInfo.m_ball, &Ball::wallCollisionOccurred, [this]() { emit collisionDetected(); });
    }

public:
    BallsCollision::ECollisionStrategyType type() const override { return eCST_PerfectlyInElastic; }

    void handleCollisions(BallsCollision *) override
    {
        if (!m_collided)
        {
            m_redBallInfo.m_ball->update(Config::MILLISECONDS_PER_FRAME);
            m_blueBallInfo.m_ball->update(Config::MILLISECONDS_PER_FRAME);

            if (m_redBallInfo.m_ball->collidesWithItem(m_blueBallInfo.m_ball))
            {
                emit collisionDetected();

                // Calculate the total mass of the system
                qreal totalMass = m_redBallInfo.m_ball->mass() + m_blueBallInfo.m_ball->mass();

                // Calculate the new velocity of the combined body
                QPointF combinedVelocity = (m_redBallInfo.m_ball->mass() * m_redBallInfo.m_ball->velocity() +
                                            m_blueBallInfo.m_ball->mass() * m_blueBallInfo.m_ball->velocity()) /
                                            totalMass;

                // Set the new velocity for both balls
                m_redBallInfo.m_ball->setVelocity(combinedVelocity);
                m_blueBallInfo.m_ball->setVelocity(combinedVelocity);

                m_collided = true;
            }
        }
        else
        {
            m_redBallInfo.m_ball->update(Config::MILLISECONDS_PER_FRAME, m_blueBallInfo.m_ball);
            m_blueBallInfo.m_ball->update(Config::MILLISECONDS_PER_FRAME, m_redBallInfo.m_ball);
        }
    }

private:
    BallsCollision::BallInfo m_redBallInfo, m_blueBallInfo;
    QGraphicsScene *m_scene;
    bool m_collided; // if true, both balls should move as one body
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BallsCollision::changeCollisionStrategy(ECollisionStrategyType strategyType)
{
    // probably can use m_collisionStrategy->deleteLater(), but it causes crash
    // m_collisionStrategy->blockSignals(true);

    switch (strategyType)
    {
        case eCST_PerfectlyElastic:
        {
            m_collisionStrategy = new CPerfectlyElasticCollisionStrategy(m_redBallInfo, m_blueBallInfo, m_scene);
            break;
        }
        case eCST_PerfectlyInElastic:
        {
            m_collisionStrategy = new CPerfectlyInElasticCollisionStrategy(m_redBallInfo, m_blueBallInfo, m_scene);
            break;
        }
        default:
            qWarning() << "Unrecognized collision strategy type.";
    }
}


BallsCollision::ECollisionStrategyType BallsCollision::collisionStrategy() const
{
    return m_collisionStrategy->type();
}


#include "collisionStrategy.moc"

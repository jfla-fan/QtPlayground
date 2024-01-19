#pragma once

#include "mainWindow.h"


class ICollisionStrategy : public QObject
{
    Q_OBJECT
public:
    ICollisionStrategy(QObject *parent = nullptr) : QObject(parent) { }
    virtual ~ICollisionStrategy() { };

    virtual BallsCollision::ECollisionStrategyType type() const = 0;

public slots:
    virtual void handleCollisions(BallsCollision *context) = 0;

signals:
    void collisionDetected();
};

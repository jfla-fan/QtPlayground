#pragma once

#include <chrono>
#include <QPointF>


namespace Config
{
    static constexpr std::chrono::milliseconds MILLISECONDS_PER_FRAME { 16 }; // 60 frames per second

    static constexpr QPointF                   RED_BALL_DEFAULT_START_POSITION { 50.0, 50.0 };
    static constexpr QPointF                   RED_BALL_DEFAULT_VELOCITY { 100.0, 100.0 }; // smth / per second

    static constexpr QPointF                   BLUE_BALL_DEFAULT_START_POSITION { 150.0, 100.0 };
    static constexpr QPointF                   BLUE_BALL_DEFAULT_VELOCITY { -100.0, 0.0 }; // smth / per second

    static constexpr qreal                     BALL_DEFAULT_MASS { 5.0 };
    static constexpr qreal                     BALL_DEFAULT_RADIUS { 20.0 };

    static constexpr qreal                     BALL_VELOCITY_SCALE { 1.0 / 1.0 };
    static constexpr qreal                     BALL_IMPULSE_MOMENTUM_SCALE { 1.0 / 10 };
}

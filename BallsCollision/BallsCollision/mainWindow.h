#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainWindow.h"


QT_BEGIN_NAMESPACE
namespace Ui { class BallsCollisionClass; };
QT_END_NAMESPACE

class Ball;
class Arrow;
class ICollisionStrategy;

class BallsCollision : public QMainWindow
{
    Q_OBJECT

public:

    enum ECollisionStrategyType
    {
        eCST_PerfectlyElastic,
        eCST_PerfectlyInElastic,
    };
    Q_ENUM(ECollisionStrategyType)

    struct BallInfo
    {
        Ball* m_ball;
        Arrow *m_ballSpeedArrow;
        Arrow *m_firstImpulseMomentumArrow;

        QPointF m_startPoint;
        QPointF m_startVelocity;
        qreal m_startRadius;
        qreal m_startMass;
    };

public:
    explicit BallsCollision(ECollisionStrategyType strategyType, QWidget *parent = nullptr);
    ~BallsCollision();

public slots:
    void simulateCollision();
    void start();
    void stop();
    void continueProcessing();
    void reset();
    void changeCollisionStrategy(ECollisionStrategyType strategyType);

public:
    ECollisionStrategyType collisionStrategy() const;

protected:
    void showEvent(QShowEvent* event) override;

private:
    BallInfo createBall(const QPointF& startPoint, const QPointF& velocity,
                        qreal radius, qreal mass, const QPen& pen, const QBrush& brush);

    void createBallConnections(Ball *ball, QSlider *speedSlider, QSlider *angleSlider, QSlider *xSlider,
                               QSlider *ySlider, QSlider *radiusSlider, QSlider *massSlider, QLabel *speedLabel,
                               QLabel *angleLabel, QLabel *xLabel, QLabel *yLabel, QLabel *radiusLabel, QLabel *massLabel);

    QGraphicsScene* createVectorScene(QGraphicsView *graphicsView);
    void createVectorScenes();

    void resetBalls();
    void resetSliders();
    void enableSliders(bool enable);

    void resetSliders(Ball *ball, QSlider *speedSlider, QSlider *angleSlider, QSlider *xSlider,
                      QSlider *ySlider, QSlider *radiusSlider, QSlider *massSlider,  QLabel *speedLabel,
                      QLabel *angleLabel, QLabel *xLabel, QLabel *yLabel, QLabel *radiusLabel, QLabel *massLabel);

    void resetVectors();

    Ui::BallsCollisionClass *m_ui;

    QTimer* m_frameTimer;
    QGraphicsScene *m_scene;

    struct State
    {
        enum ESimulationStatus
        {
            eSS_Initialized,
            eSS_Processing,
            eSS_Stopped,
        }
        m_status { eSS_Initialized };
    }
    m_state;

    ICollisionStrategy *m_collisionStrategy;
    BallInfo m_redBallInfo, m_blueBallInfo;

    QGraphicsScene *m_speedScene;
    QGraphicsScene *m_momentumScene;
};

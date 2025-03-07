#include "mainWindow.h"

#include <QtWidgets>
#include <qDebug>

#include "config.hpp"
#include "ball.h"
#include "arrow.h"
#include "collisionStrategy.h"



BallsCollision::BallsCollision(ECollisionStrategyType strategyType, QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::BallsCollisionClass())
    , m_frameTimer(new QTimer())
    , m_collisionStrategy(nullptr)
{
    m_ui->setupUi(this);

    QRect sceneArea { m_ui->graphicsView->geometry().x(), m_ui->graphicsView->geometry().y(),
                      m_ui->graphicsView->geometry().width(), m_ui->graphicsView->geometry().height() };

    qDebug() << "Scene area: " << sceneArea;

    m_scene = new QGraphicsScene(sceneArea, this);

    qDebug() << "Before vector scenes.";
    createVectorScenes();

    qDebug() << "Before creating balls";

    m_redBallInfo  = createBall(Config::RED_BALL_DEFAULT_START_POSITION, Config::RED_BALL_DEFAULT_VELOCITY,
                                Config::BALL_DEFAULT_RADIUS, Config::BALL_DEFAULT_MASS, QPen(Qt::red), QBrush(Qt::red));

    m_blueBallInfo = createBall(Config::BLUE_BALL_DEFAULT_START_POSITION, Config::BLUE_BALL_DEFAULT_VELOCITY,
                                Config::BALL_DEFAULT_RADIUS, Config::BALL_DEFAULT_MASS, QPen(Qt::blue), QBrush(Qt::blue));

    m_scene->addItem(m_redBallInfo.m_ball);
    m_scene->addItem(m_blueBallInfo.m_ball);

    changeCollisionStrategy(strategyType);

    createBallConnections(m_redBallInfo.m_ball,
                          m_ui->speedSlider1, m_ui->angleSlider1, m_ui->xSlider1, m_ui->ySlider1, m_ui->radiusSlider1, m_ui->massSlider1,
                          m_ui->speedLabel1, m_ui->angleLabel1, m_ui->xLabel1, m_ui->yLabel1, m_ui->radiusLabel1, m_ui->massLabel1);

    createBallConnections(m_blueBallInfo.m_ball,
                          m_ui->speedSlider2, m_ui->angleSlider2, m_ui->xSlider2, m_ui->ySlider2, m_ui->radiusSlider2, m_ui->massSlider2,
                          m_ui->speedLabel2, m_ui->angleLabel2, m_ui->xLabel2, m_ui->yLabel2, m_ui->radiusLabel2, m_ui->massLabel2);

    connect(m_ui->startButton,     &QPushButton::clicked,           this, &BallsCollision::start);
    connect(m_ui->stopButton,      &QPushButton::clicked,           this, &BallsCollision::stop);
    connect(m_ui->continueButton,  &QPushButton::clicked,           this, &BallsCollision::continueProcessing);
    connect(m_ui->resetButton,     &QPushButton::clicked,           this, &BallsCollision::reset);

    connect(m_ui->modeComboBox,    &QComboBox::currentIndexChanged, this, [this](int index) { changeCollisionStrategy(ECollisionStrategyType(index)); });

    setWindowTitle(tr("Balls collision simulator"));
}


BallsCollision::~BallsCollision()
{
    delete m_ui;
}

void BallsCollision::simulateCollision()
{
    if (m_collisionStrategy)
    {
        m_collisionStrategy->handleCollisions(this);
    }
}

void BallsCollision::start()
{
    if (m_state.m_status == State::eSS_Initialized)
    {
        m_redBallInfo.m_ball->setFlag(QGraphicsItem::ItemIsMovable, false);
        m_blueBallInfo.m_ball->setFlag(QGraphicsItem::ItemIsMovable, false);

        continueProcessing();
    }
}

void BallsCollision::stop()
{
    if (m_state.m_status == State::eSS_Processing)
    {
        disconnect(m_frameTimer, &QTimer::timeout, this, &BallsCollision::simulateCollision);
        m_frameTimer->stop();

        m_state.m_status = State::eSS_Stopped;
    }
}

void BallsCollision::continueProcessing()
{
    if (m_state.m_status == State::eSS_Stopped || m_state.m_status == State::eSS_Initialized)
    {
        connect(m_frameTimer, &QTimer::timeout, this, &BallsCollision::simulateCollision);
        m_frameTimer->start(Config::MILLISECONDS_PER_FRAME);

        m_state.m_status = State::eSS_Processing;
    }
}

void BallsCollision::reset()
{
    if (m_state.m_status != State::eSS_Initialized)
    {
        stop();
        resetBalls();
        resetSliders();
        resetVectors();

        changeCollisionStrategy(collisionStrategy());

        m_state.m_status = State::eSS_Initialized;
    }
}

void BallsCollision::showEvent(QShowEvent* event)
{
    m_ui->graphicsView->setScene(m_scene);

    auto sceneRect = m_ui->graphicsView->sceneRect();
    qDebug() << sceneRect.x() << " " << sceneRect.y() << " " << sceneRect.width() << " " << sceneRect.height();
}

BallsCollision::BallInfo BallsCollision::createBall(const QPointF& startPoint, const QPointF& velocity,
                                                    qreal radius, qreal mass, const QPen& pen, const QBrush& brush)
{
    Ball *ball = new Ball(startPoint, velocity, radius, mass);
    ball->setPen(pen);
    ball->setBrush(brush);
    ball->setFlag(QGraphicsItem::ItemIsMovable, true);

    QPointF momentumSceneCenter = { m_momentumScene->width() / 2, m_momentumScene->height() / 2 };
    QPointF speedSceneCenter = { m_momentumScene->width() / 2, m_momentumScene->height() / 2 };


    Arrow *speedArrow = Arrow::fromVelocity(speedSceneCenter, speedSceneCenter + velocity);
    speedArrow->setPen(pen);

    m_speedScene->addItem(speedArrow);

    qDebug() << "Ball speed arrow: " << speedArrow->line().p1() << " " << speedArrow->line().p2();

    Arrow *firstImpulseMomentumArrow = Arrow::fromPoints(momentumSceneCenter, ball->mass() * velocity + momentumSceneCenter);
    firstImpulseMomentumArrow->setPen(pen);

    m_momentumScene->addItem(firstImpulseMomentumArrow);

    auto changeArrows = [=](const QPointF& velocity, qreal mass)
    {
        qDebug() << "Got new velocity and mass and angle: " << velocity << " " << mass << ball->velocityAngle();
        speedArrow->resetFromPoints(speedSceneCenter, speedSceneCenter + velocity * Config::BALL_VELOCITY_SCALE);
        firstImpulseMomentumArrow->resetFromPoints(momentumSceneCenter, momentumSceneCenter + mass * velocity * Config::BALL_IMPULSE_MOMENTUM_SCALE);
    };

    qDebug() << "Ball impulse momentum arrow: " << firstImpulseMomentumArrow->line().p1() << " " << firstImpulseMomentumArrow->line().p2();

    connect(ball, &Ball::velocityChanged, this, [=](const QPointF& newVelocity) { changeArrows(newVelocity, ball->mass()); });
    connect(ball, &Ball::massChanged, this, [=](qreal newMass) { changeArrows(ball->velocity(), newMass); });

    return { ball, speedArrow, firstImpulseMomentumArrow, startPoint, velocity, radius, mass };
}

void BallsCollision::createBallConnections(Ball *ball, QSlider *speedSlider, QSlider *angleSlider, QSlider *xSlider,
                                           QSlider *ySlider, QSlider *radiusSlider, QSlider *massSlider, QLabel *speedLabel,
                                           QLabel *angleLabel, QLabel *xLabel, QLabel *yLabel, QLabel *radiusLabel, QLabel *massLabel)
{
    speedSlider->setMinimum(-1000);
    speedSlider->setMaximum(1000);

    angleSlider->setMinimum(-180);
    angleSlider->setMaximum(180);

    xSlider->setMinimum(0);
    xSlider->setMaximum(m_scene->sceneRect().width());

    ySlider->setMinimum(0);
    ySlider->setMaximum(m_scene->sceneRect().height());

    resetSliders(ball, speedSlider, angleSlider, xSlider, ySlider, radiusSlider, massSlider,
                 speedLabel, angleLabel, xLabel, yLabel, radiusLabel, massLabel);

    connect(speedSlider,         &QSlider::valueChanged, this, [=](int newValue) { if (speedSlider->isSliderDown()) { ball->changeVelocityLength(newValue); speedLabel->setText(QString::number(newValue)); } });

    // probably velocity changed after collision
    //connect(m_collisionStrategy, &ICollisionStrategy::collisionDetected, this, [=]() { speedSlider->setValue(ball->velocityLength()); });
    //connect(m_collisionStrategy, &ICollisionStrategy::collisionDetected, this, [=]() { angleSlider->setValue(qRadiansToDegrees(ball->velocityAngle())); });
    qDebug() << "Ball's angle and length: " << qRadiansToDegrees(ball->velocityAngle()) << " " << ball->velocityLength();

    connect(angleSlider,         &QSlider::valueChanged, this, [=](int newValue) { if (angleSlider->isSliderDown()) { ball->changeVelocityAngle(qDegreesToRadians(-newValue)); angleLabel->setText(QString::number(newValue)); } });
    connect(xSlider,             &QSlider::valueChanged, this, [=](int newValue) { if (xSlider->isSliderDown()) { ball->setPosition(newValue, ball->scenePos().y()); xLabel->setText(QString::number(newValue)); } });
    connect(ySlider,             &QSlider::valueChanged, this, [=](int newValue) { if (ySlider->isSliderDown()) { ball->setPosition(ball->scenePos().x(), newValue); yLabel->setText(QString::number(newValue)); } });
    connect(radiusSlider,        &QSlider::valueChanged, this, [=](int newValue) { if (radiusSlider->isSliderDown()) { ball->setRadius(newValue); radiusLabel->setText(QString::number(newValue)); } });
    connect(massSlider,          &QSlider::valueChanged, this, [=](int newValue) { if (massSlider->isSliderDown()) { ball->setMass(newValue); massLabel->setText(QString::number(newValue)); } });

    connect(ball, &Ball::velocityChanged, this, [=](const QPointF&) { speedSlider->setValue(ball->velocityLength()); speedLabel->setText(QString::number(speedSlider->value()));
                                                                      angleSlider->setValue(qRadiansToDegrees(-ball->velocityAngle())); angleLabel->setText(QString::number(angleSlider->value())); });
    connect(ball, &Ball::posChanged, this, [=](const QPointF&) { xSlider->setValue(ball->scenePos().x()); xLabel->setText(QString::number(xSlider->value()));
                                                                 ySlider->setValue(ball->scenePos().y()); yLabel->setText(QString::number(ySlider->value())); });
}

QGraphicsScene* BallsCollision::createVectorScene(QGraphicsView *graphicsView)
{
    QGraphicsScene *scene = new QGraphicsScene(graphicsView->rect(), this);
    qDebug() << "Vector scene geometry: " << scene->sceneRect();
    qDebug() << "rect left, right, top bottom: " << scene->sceneRect().left() << " " <<
                                                    scene->sceneRect().right() << " " <<
                                                    scene->sceneRect().top() << " " <<
                                                    scene->sceneRect().bottom();
    // Draw X and Y axes
    //Arrow *xAxis = new Arrow();
    Arrow *xAxis = Arrow::fromPoints(QPointF { scene->sceneRect().left(), scene->sceneRect().bottom() / 2 },
                                     QPointF { scene->sceneRect().right(), scene->sceneRect().height() / 2 });
    xAxis->setPen(QPen(Qt::black));
    scene->addItem(xAxis);

    qDebug() << "X axis points: " << xAxis->line().p1() << " " << xAxis->line().p2();

    Arrow *yAxis = Arrow::fromPoints(QPointF { scene->width() / 2, scene->height() }, QPointF { scene->width() / 2, 0 });
    yAxis->setPen(QPen(Qt::black));
    scene->addItem(yAxis);

    qDebug() << "Y axis points: " << yAxis->line().p1() << " " << yAxis->line().p2();

    graphicsView->setScene(scene);

    return scene;
}

void BallsCollision::createVectorScenes()
{
    m_speedScene = createVectorScene(m_ui->speedVectorsView);
    m_momentumScene = createVectorScene(m_ui->momentumVectorsView);
}

void BallsCollision::resetBalls()
{
    m_redBallInfo.m_ball->reset(m_redBallInfo.m_startPoint, m_redBallInfo.m_startVelocity,
                                m_redBallInfo.m_startRadius, m_redBallInfo.m_startMass);

    m_blueBallInfo.m_ball->reset(m_blueBallInfo.m_startPoint, m_blueBallInfo.m_startVelocity,
                                 m_blueBallInfo.m_startRadius, m_blueBallInfo.m_startMass);
}

void BallsCollision::resetSliders()
{
    resetSliders(m_redBallInfo.m_ball, m_ui->speedSlider1, m_ui->angleSlider1, m_ui->xSlider1, m_ui->ySlider1, m_ui->radiusSlider1, m_ui->massSlider1,
                 m_ui->speedLabel1, m_ui->angleLabel1, m_ui->xLabel1, m_ui->yLabel1, m_ui->radiusLabel1, m_ui->massLabel1);

    resetSliders(m_blueBallInfo.m_ball, m_ui->speedSlider2, m_ui->angleSlider2, m_ui->xSlider2, m_ui->ySlider2, m_ui->radiusSlider2, m_ui->massSlider2,
                 m_ui->speedLabel2, m_ui->angleLabel2, m_ui->xLabel2, m_ui->yLabel2, m_ui->radiusLabel2, m_ui->massLabel2);
}

void BallsCollision::enableSliders(bool enable)
{
    m_ui->speedSlider1->setEnabled(enable);
    m_ui->angleSlider1->setEnabled(enable);
    m_ui->xSlider1->setEnabled(enable);
    m_ui->ySlider1->setEnabled(enable);
    m_ui->radiusSlider1->setEnabled(enable);
    m_ui->massSlider1->setEnabled(enable);

    m_ui->speedSlider2->setEnabled(enable);
    m_ui->angleSlider2->setEnabled(enable);
    m_ui->xSlider2->setEnabled(enable);
    m_ui->ySlider2->setEnabled(enable);
    m_ui->radiusSlider2->setEnabled(enable);
    m_ui->massSlider2->setEnabled(enable);
}

void BallsCollision::resetSliders(Ball *ball, QSlider *speedSlider, QSlider *angleSlider, QSlider *xSlider,
                                  QSlider *ySlider, QSlider *radiusSlider, QSlider *massSlider, QLabel *speedLabel,
                                  QLabel *angleLabel, QLabel *xLabel, QLabel *yLabel, QLabel *radiusLabel, QLabel *massLabel)
{
    speedSlider->setSliderPosition(ball->velocityLength());
    speedLabel->setText(QString::number(speedSlider->value()));

    angleSlider->setSliderPosition(qRadiansToDegrees(-ball->velocityAngle()));
    angleLabel->setText(QString::number(angleSlider->value()));

    xSlider->setSliderPosition(ball->scenePos().x());
    xLabel->setText(QString::number(xSlider->value()));

    ySlider->setSliderPosition(ball->scenePos().y());
    yLabel->setText(QString::number(ySlider->value()));

    radiusSlider->setSliderPosition(ball->radius());
    radiusLabel->setText(QString::number(radiusSlider->value()));

    massSlider->setSliderPosition(ball->mass());
    massLabel->setText(QString::number(massSlider->value()));
}

void BallsCollision::resetVectors()
{

}

#include "Scene.h"

#include "Pacman.h"
#include "Sprite.h"
#include "SpriteSheet.h"
#include "Map.h"

#include <QKeyEvent>

namespace
{
const float frame_time = 1.0f / 60.0f;

float msToSec(quint64 ms)
{
    return ms / 1000.0f;
}

quint64 secToMs(float sec)
{
    return sec * 1000.0f;
}
}

Scene::Scene(QObject* parent)
    : QGraphicsScene(parent)
{
    Map* map = new Map;
    map->load(":/assets/map.tmx");
    map->setScale(2);
    addItem(map);

    SpriteSheet* sh = new SpriteSheet;
    sh->load(":/assets/pacman.tsx");

    pacman_ = new Pacman(sh);
    pacman_->setSpeed(50);
    pacman_->setScale(2);
    addItem(pacman_);

    timer_ = new QTimer(this);
    timer_->start(secToMs(frame_time));
    connect(timer_, &QTimer::timeout, this, &Scene::update);

    elapsed_timer_.start();
}

void Scene::keyPressEvent(QKeyEvent* event)
{
    if (!event->isAutoRepeat())
    {
        switch (event->key())
        {
        case Qt::Key_Left:
        case Qt::Key_A:
            pacman_->setState(Pacman::Left);
            break;
        case Qt::Key_Right:
        case Qt::Key_D:
            pacman_->setState(Pacman::Right);
            break;
        case Qt::Key_Up:
        case Qt::Key_W:
            pacman_->setState(Pacman::Up);
            break;
        case Qt::Key_Down:
        case Qt::Key_S:
            pacman_->setState(Pacman::Down);
            break;
        default:
            break;
        }
    }

    QGraphicsScene::keyPressEvent(event);
}

void Scene::keyReleaseEvent(QKeyEvent* event)
{
}

void Scene::update()
{
    delta_time_sec_ = msToSec(elapsed_timer_.elapsed());
    elapsed_timer_.restart();

    pacman_->move(delta_time_sec_);

    QGraphicsScene::update();
}

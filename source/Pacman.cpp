#include "Pacman.h"

#include "SpriteSheet.h"
#include "Sprite.h"

#include <QDebug>

Pacman::Pacman(SpriteSheet* sheet, QObject* parent)
    : QObject(parent)
{
    sprites_[Idle] = sheet->getSprite("pacman_idle");
    sprites_[Up] = sheet->getSprite("pacman_up");
    sprites_[Down] = sheet->getSprite("pacman_down");
    sprites_[Left] = sheet->getSprite("pacman_left");
    sprites_[Right] = sheet->getSprite("pacman_right");
    sprites_[Destroy] = sheet->getSprite("pacman_destroy");
}

void Pacman::move(float dt)
{
    setPos(pos() + direction_ * speed_ * dt);
    update();
}

void Pacman::setSpeed(float speed)
{
    speed_ = speed;
}

void Pacman::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    sprites_[state_]->setPos(pos());
    sprites_[state_]->paint(painter, option, widget);
}

QRectF Pacman::boundingRect() const
{
    sprites_[state_]->setPos(pos());
    return sprites_[state_]->boundingRect();
}

void Pacman::setState(State state)
{
    state_ = state;

    switch (state_)
    {
    case Pacman::Idle:
        direction_ = QPointF(0, 0);
        break;
    case Pacman::Up:
        direction_ = QPointF(0, -1);
        break;
    case Pacman::Down:
        direction_ = QPointF(0, 1);
        break;
    case Pacman::Left:
        direction_ = QPointF(-1, 0);
        break;
    case Pacman::Right:
        direction_ = QPointF(1, 0);
        break;
    case Pacman::Destroy:
        direction_ = QPointF(0, 0);
        break;
    case Pacman::StatesCount:
        assert(false);
        break;
    default:
        break;
    }
}

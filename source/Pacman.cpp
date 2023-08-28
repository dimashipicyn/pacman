#include "Pacman.h"

#include "Tile.h"

#include <QDebug>

MovableSprite::MovableSprite(Tile* idle_sprite,
    Tile* up_sprite,
    Tile* down_sprite,
    Tile* left_sprite,
    Tile* right_sprite,
    Tile* destroy_sprite,
    QObject* parent)
    : QObject(parent)
    , QGraphicsItem()
{
    sprites_[Idle] = idle_sprite;
    sprites_[Up] = up_sprite;
    sprites_[Down] = down_sprite;
    sprites_[Left] = left_sprite;
    sprites_[Right] = right_sprite;
    sprites_[Destroy] = destroy_sprite;

    directions_[Idle] = QPointF(0, 0);
    directions_[Up] = QPointF(0, -1);
    directions_[Down] = QPointF(0, 1);
    directions_[Left] = QPointF(-1, 0);
    directions_[Right] = QPointF(1, 0);
    directions_[Destroy] = QPointF(0, 0);

    setSize(sprites_[Idle]->size());
}

void MovableSprite::move(float dt)
{
    setPos(pos() + directions_[state_] * speed_ * dt);
    update();
}

void MovableSprite::setSpeed(float speed)
{
    speed_ = speed;
}

void MovableSprite::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    sprites_[state_]->setPos(pos());
    sprites_[state_]->paint(painter, option, widget);
}

QRectF MovableSprite::boundingRect() const
{
    return sprites_[state_]->boundingRect();
}

void MovableSprite::setState(State state)
{
    state_ = state;
    assert(state > BeginState && state < StatesCount && "Wrong state");
}

void MovableSprite::setSize(QSize size)
{
    for (int i = BeginState + 1; i < StatesCount; ++i)
    {
        sprites_[i]->setSize(size);
    }
}

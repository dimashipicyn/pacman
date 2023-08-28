#pragma once

#include <QObject>
#include <QVector>
#include <QGraphicsItem>

class Tile;

class MovableSprite : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    MovableSprite(
        Tile* idle_sprite,
        Tile* up_sprite,
        Tile* down_sprite,
        Tile* left_sprite,
        Tile* right_sprite,
        Tile* destroy_sprite,
        QObject* parent = nullptr);

    enum State
    {
        BeginState = -1,
        Idle,
        Up,
        Down,
        Left,
        Right,
        Destroy,
        StatesCount
    };

    void move(float dt);
    void setSpeed(float speed);
    void setState(State state);
    void setSize(QSize size);

private:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    QRectF boundingRect() const override;

private:
    Tile* sprites_[StatesCount] = { nullptr };
    QPointF directions_[StatesCount] = {};
    State state_ = Idle;
    float speed_ = 0;
};
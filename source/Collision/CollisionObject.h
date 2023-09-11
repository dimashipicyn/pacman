#pragma once

#include <QPointF>

class Collider;

struct CollisionObject
{
    QPointF position;
    Collider* collider = nullptr;
};
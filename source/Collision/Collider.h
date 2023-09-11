#pragma once

#include "CollisionPoints.h"

#include <QPointF>

class RectangleCollider;

class Collider
{
public:
    virtual void setPosition(QPointF position) = 0;
    virtual CollisionPoints intersects(const Collider* other) const = 0;
    virtual CollisionPoints intersects(const RectangleCollider* other) const = 0;
};
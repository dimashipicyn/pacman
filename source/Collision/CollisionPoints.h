#pragma once

#include <QPointF>

struct CollisionPoints
{
    CollisionPoints() { }
    CollisionPoints(QPointF normal, float depth)
        : normal { normal }
        , depth { depth }
        , hasCollision { true }
    {
    }
    QPointF normal;
    float depth = 0.0f;
    bool hasCollision = false;
};
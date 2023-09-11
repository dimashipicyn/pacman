#pragma once

#include <QPointF>

#include "CollisionPoints.h"

class CollisionObject;

struct CollisionManifold
{
    CollisionManifold() { }
    CollisionManifold(CollisionObject* a, CollisionObject* b, CollisionPoints collisionPoints)
        : a { a }
        , b { b }
        , collisionPoints { std::move(collisionPoints) }
    {
        
    }

    CollisionObject* a = nullptr;
    CollisionObject* b = nullptr;
    CollisionPoints collisionPoints;
};
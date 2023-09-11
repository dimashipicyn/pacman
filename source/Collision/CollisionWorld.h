#pragma once

#include "CollisionObject.h"
#include "CollisionManifold.h"

#include <QVector>

class CollisionWorld
{
public:
    void addCollisionObject(CollisionObject* obj);

    void update(float dt);

private:
    void resolveCollision(QVector<CollisionManifold>& collisions);

private:
    QVector<CollisionObject*> objects_;
};
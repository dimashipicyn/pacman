#include "CollisionWorld.h"

#include "Collider.h"
#include "CollisionPoints.h"
#include "CollisionManifold.h"

void CollisionWorld::addCollisionObject(CollisionObject* obj)
{
    objects_.push_back(obj);
}

void CollisionWorld::update(float dt)
{
    QVector<CollisionManifold> collisions;
    for (CollisionObject* a : objects_)
    {
        a->collider->setPosition(a->position);
        for (CollisionObject* b : objects_)
        {
            if (a == b)
            {
                continue;
            }

            b->collider->setPosition(b->position);
            
            if (CollisionPoints cp = a->collider->intersects(b->collider); cp.hasCollision)
            {
                collisions.push_back(CollisionManifold(a, b, cp));
            }
        }
    }

    resolveCollision(collisions);
}

void CollisionWorld::resolveCollision(QVector<CollisionManifold>& collisions)
{
    for (CollisionManifold& cm : collisions)
    {
        cm.a->position += cm.collisionPoints.normal * (cm.collisionPoints.depth / 2.0f);
        cm.b->position -= cm.collisionPoints.normal * (cm.collisionPoints.depth / 2.0f);
    }
}

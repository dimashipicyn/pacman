#include "Entity.h"

const QPointF& Entity::getPos() const
{
    return pos_;
}

void Entity::setPos(const QPointF& pos)
{
    pos_ = pos;
}

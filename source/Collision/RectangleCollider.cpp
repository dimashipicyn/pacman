#include "RectangleCollider.h"

RectangleCollider::RectangleCollider(QRectF rect)
    : rect_ { rect }
{
}

void RectangleCollider::setPosition(QPointF position)
{
    rect_ = QRectF(position.x(), position.y(), rect_.width(), rect_.height());
}

CollisionPoints RectangleCollider::intersects(const Collider* other) const
{
    return other->intersects(this);
}

CollisionPoints RectangleCollider::intersects(const RectangleCollider* other) const
{
    if (auto intersected = rect_.intersected(other->rect_); intersected.isValid())
    {
        QPointF normal = other->rect_.center() - rect_.center();
        float depth = intersected.width() < intersected.height() ? intersected.width() : intersected.height();
        return CollisionPoints(normal, depth);
    }
    return CollisionPoints();
}

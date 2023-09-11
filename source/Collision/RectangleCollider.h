#pragma once

#include "Collider.h"

#include <QRectF>

class RectangleCollider final : public Collider
{
public:
    RectangleCollider(QRectF rect);

    void setPosition(QPointF position) override;
    CollisionPoints intersects(const Collider* other) const override;

private:
    CollisionPoints intersects(const RectangleCollider* other) const override;

private:
    QRectF rect_;
};
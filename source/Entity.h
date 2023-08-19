#include <QPointF>

class Entity
{
public:
    virtual void input() {};
    virtual void update(float dt) {};

    const QPointF& getPos() const;
    void setPos(const QPointF& pos);

private:
    QPointF pos_;
};
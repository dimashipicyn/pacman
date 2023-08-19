#include <QObject>
#include <QVector>
#include <QGraphicsItem>

class SpriteSheet;
class Sprite;

class Pacman : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Pacman(SpriteSheet* sheet, QObject* parent = nullptr);

    enum State
    {
        Idle,
        Up,
        Down,
        Left,
        Right,
        Destroy,
        StatesCount
    };

    void move(float dt);
    void setSpeed(float speed);
    void setState(State state);

private:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    QRectF boundingRect() const override;

    void updateSprite();

    void connectSprite();
    void disconnectSprite();

private:
    Sprite* sprites_[StatesCount] = { nullptr };
    State state_ = Up;
    QPointF direction_;
    float speed_ = 0;
};
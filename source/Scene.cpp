#include "Scene.h"

#include "Pacman.h"
#include "TsxTileset.h"
#include "TmxMap.h"
#include "Tile.h"

#include <QKeyEvent>

namespace
{
const float frame_time = 1.0f / 60.0f;

float msToSec(quint64 ms)
{
    return ms / 1000.0f;
}

quint64 secToMs(float sec)
{
    return sec * 1000.0f;
}
}

Scene::Scene(QObject* parent)
    : QGraphicsScene(parent)
{
    map_ = new TmxMap(this);
    map_->load(":/assets/map.tmx");
    addItem(map_);

    create_objects();

    timer_ = new QTimer(this);
    timer_->start(secToMs(frame_time));
    connect(timer_, &QTimer::timeout, this, &Scene::update);

    elapsed_timer_.start();
}

void Scene::keyPressEvent(QKeyEvent* event)
{
    if (!event->isAutoRepeat())
    {
        switch (event->key())
        {
        case Qt::Key_Left:
        case Qt::Key_A:
            pacman_->setState(MovableSprite::Left);
            break;
        case Qt::Key_Right:
        case Qt::Key_D:
            pacman_->setState(MovableSprite::Right);
            break;
        case Qt::Key_Up:
        case Qt::Key_W:
            pacman_->setState(MovableSprite::Up);
            break;
        case Qt::Key_Down:
        case Qt::Key_S:
            pacman_->setState(MovableSprite::Down);
            break;
        default:
            break;
        }
    }

    QGraphicsScene::keyPressEvent(event);
}

void Scene::keyReleaseEvent(QKeyEvent* event)
{
}

void Scene::update()
{
    delta_time_sec_ = msToSec(elapsed_timer_.elapsed());
    elapsed_timer_.restart();

    pacman_->move(delta_time_sec_);

    QGraphicsScene::update();
}

class CustomItem : public QGraphicsItem
{
public:
    CustomItem(QGraphicsItem* parent = nullptr)
        : QGraphicsItem(parent)
    {
        // Set the item's size and properties
        itemSize = QSizeF(50, 50);
        setFlag(ItemIsMovable);
    }

    QRectF boundingRect() const override
    {
        return QRectF(0, 0, itemSize.width(), itemSize.height());
    }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override
    {
        // Define how to paint the item
        painter->setBrush(Qt::blue); // Fill color
        painter->setPen(Qt::black); // Outline color
        painter->drawRect(boundingRect()); // Draw a rectangle
    }

private:
    QSizeF itemSize;
};

void Scene::create_objects()
{
    TmxObject obj = map_->getObject("pacman");
    
    TsxTileset* sheet = new TsxTileset(this);
    sheet->load(":/assets/" + obj.getProperty("tileset"));

    pacman_ = new MovableSprite(
        sheet->getTile(obj.getProperty("idle_tile")),
        sheet->getTile(obj.getProperty("up_tile")),
        sheet->getTile(obj.getProperty("down_tile")),
        sheet->getTile(obj.getProperty("left_tile")),
        sheet->getTile(obj.getProperty("right_tile")),
        sheet->getTile(obj.getProperty("destroy_tile")),
        this);

    pacman_->setPos(obj.rect.x(), obj.rect.y());
    pacman_->setSize(obj.rect.size());
    pacman_->setSpeed(20);
    addItem(pacman_);
}

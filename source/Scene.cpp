#include "Scene.h"

#include "Pacman.h"
#include "Tiled/TmxMapLoader.h"
#include "Tiled/TsxTileset.h"
#include "Map.h"
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
    Tiled::TmxMapLoader mapLoader;
    mapLoader.load(":/assets/map.tmx");
    map_ = new Map(mapLoader, this);
    addItem(map_);

    createObjects(mapLoader);

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

void Scene::createObjects(const Tiled::TmxMapLoader& mapLoader)
{
    Tiled::TmxObject obj = mapLoader.getObjectLayer("objects").getObject("pacman");

    Tiled::TsxTileset* sheet = new Tiled::TsxTileset;
    sheet->load(":/assets/" + obj.getProperty("tileset"));

    QString idleTile = obj.getProperty("idle_tile");
    QString upTile = obj.getProperty("up_tile");
    QString downTile = obj.getProperty("down_tile");
    QString leftTile = obj.getProperty("left_tile");
    QString rightTile = obj.getProperty("right_tile");
    QString destroyTile = obj.getProperty("destroy_tile");

    pacman_ = new MovableSprite(
        new Tile(sheet, sheet->getTile(idleTile)),
        new Tile(sheet, sheet->getTile(upTile)),
        new Tile(sheet, sheet->getTile(downTile)),
        new Tile(sheet, sheet->getTile(leftTile)),
        new Tile(sheet, sheet->getTile(rightTile)),
        new Tile(sheet, sheet->getTile(destroyTile)),
        this);

    pacman_->setPos(obj.position.x(), obj.position.y());
    pacman_->setSize(obj.size);
    pacman_->setSpeed(50);
    addItem(pacman_);
}

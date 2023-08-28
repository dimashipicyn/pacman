#pragma once

#include <QGraphicsScene>
#include <QTimer>
#include <QElapsedTimer>

class MovableSprite;
class Tile;
class TmxMap;

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene(QObject* parent = nullptr);

protected:
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void keyReleaseEvent(QKeyEvent* event) override;

private:
    void update();
    void create_objects();

private:
    QTimer* timer_ = nullptr;
    QElapsedTimer elapsed_timer_;

    TmxMap* map_ = nullptr;
    MovableSprite* pacman_;
    float delta_time_sec_ = 0;
};
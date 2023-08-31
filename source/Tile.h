#pragma once

#include "TsxTile.h"
#include "TsxTileset.h"
#include <QGraphicsItem>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <cstddef>

class Tile : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Tile(Tiled::TsxTileset* tileset, Tiled::TsxTile tile, QObject* parent = nullptr);

    QSize size() const;
    void setSize(QSize size);

    void playAnimation(bool play);
    void repeatAnimation(bool repeat);

    bool isPlayedAnimation() const;
    bool isEndAnimation() const;
    bool isRepeated() const;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    QRectF boundingRect() const override;

private:
    void nextFrame();

    QTimer timer_;
    QSize size_;

    Tiled::TsxTileset* tileset_ = nullptr;
    Tiled::TsxTile tile_;
    
    int currentFrame_ = 0;
    bool playedAnimation_ = true;
    bool repeatedAnimation_ = true;
};
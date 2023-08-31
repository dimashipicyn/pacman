#include "Tile.h"
#include "TsxTile.h"
#include "TsxTileset.h"

Tile::Tile(Tiled::TsxTileset* tileset, Tiled::TsxTile tile, QObject* parent)
    : QObject(parent)
    , QGraphicsItem()
    , tileset_{ tileset }
    , tile_{tile}
    , currentFrame_ { rand() % tile.animationFrames() }
{
    Tiled::TsxTileAnimationFrame animFrame = tile.getAnimationFrame(currentFrame_);
    Tiled::TsxTilesetFrame frame = tileset->getFrameFromId(animFrame.tileId);
    size_ = frame.srcRect.size();

    if (tile.animationFrames() > 1)
    {
        connect(&timer_, &QTimer::timeout, this, &Tile::nextFrame);
        timer_.start(animFrame.duration);
    }
}

void Tile::playAnimation(bool play)
{
    playedAnimation_ = play;
}

void Tile::repeatAnimation(bool repeat)
{
    repeatedAnimation_ = repeat;
}

bool Tile::isPlayedAnimation() const
{
    return playedAnimation_;
}

bool Tile::isEndAnimation() const
{
    return !isRepeated() && !isPlayedAnimation();
}

bool Tile::isRepeated() const
{
    return repeatedAnimation_;
}

void Tile::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Tiled::TsxTileAnimationFrame animFrame = tile_.getAnimationFrame(currentFrame_);
    Tiled::TsxTilesetFrame frame = tileset_->getFrameFromId(animFrame.tileId);
    painter->drawPixmap(boundingRect(), frame.image, frame.srcRect);
}

QRectF Tile::boundingRect() const
{
    return QRectF(0, 0, size_.width(), size_.height());
}

void Tile::nextFrame()
{
    ++currentFrame_;
    currentFrame_ %= tile_.animationFrames();

    timer_.setInterval(tile_.getAnimationFrame(currentFrame_).duration);

    update(boundingRect());
}

void Tile::setSize(QSize size)
{
    size_ = size;
}

QSize Tile::size() const
{
    return size_;
}

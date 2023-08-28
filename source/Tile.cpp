#include "Tile.h"

Tile::Tile(QPixmap image, QVector<Frame> frames, QObject* parent)
    : QObject(parent)
    , QGraphicsItem()
    , image_ { image }
    , frames_ { std::move(frames) }
    , currentFrame_ { rand() % frames_.size() }
{
    if (frames_.size() > 1)
    {
        connect(&timer_, &QTimer::timeout, this, &Tile::nextFrame);
        timer_.start(frames_[currentFrame_].duration);
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
    const Frame& frame = frames_[currentFrame_];
    painter->drawPixmap(pos(), image_, frame.rect);
}

QRectF Tile::boundingRect() const
{
    const qreal penWidth = 1;
    const QRect& rect = frames_[currentFrame_].rect;
    return QRectF(pos().x() - penWidth, pos().y() - penWidth, rect.width() + penWidth, rect.height() + penWidth);
}

void Tile::nextFrame()
{
    ++currentFrame_;
    currentFrame_ %= frames_.size();
    
    timer_.setInterval(frames_[currentFrame_].duration);

    update(boundingRect());
}

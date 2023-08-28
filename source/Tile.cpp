#include "Tile.h"

Tile::Tile(QPixmap image, QVector<Frame> frames, QObject* parent)
    : QObject(parent)
    , QGraphicsItem()
    , image_ { image }
    , frames_ { std::move(frames) }
    , currentFrame_ { rand() % frames_.size() }
{
    assert(frames_.size() > 0);

    size_ = frames_.front().rect.size();

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
    painter->drawPixmap(boundingRect(), image_, frame.rect);
}

QRectF Tile::boundingRect() const
{
    return QRectF(0, 0, size_.width(), size_.height());
}

void Tile::nextFrame()
{
    ++currentFrame_;
    currentFrame_ %= frames_.size();

    timer_.setInterval(frames_[currentFrame_].duration);

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

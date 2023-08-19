#include "Sprite.h"

Sprite::Sprite(QPixmap image, QVector<Frame> frames, QObject* parent)
    : QObject(parent)
    , QGraphicsItem()
    , image_ { image }
    , frames_ { std::move(frames) }
    , currentFrame_ { rand() % frames_.size() }
{
    if (frames_.size() > 1)
    {
        connect(&timer_, &QTimer::timeout, this, &Sprite::nextFrame);
        timer_.start(frames_[currentFrame_].duration);
    }
}

void Sprite::playAnimation(bool play)
{
    playedAnimation_ = play;
}

void Sprite::repeatAnimation(bool repeat)
{
    repeatedAnimation_ = repeat;
}
 
bool Sprite::isPlayedAnimation() const
{
    return playedAnimation_;
}

bool Sprite::isEndAnimation() const
{
    return !isRepeated() && !isPlayedAnimation();
}

bool Sprite::isRepeated() const
{
    return repeatedAnimation_;
}

void Sprite::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    const Frame& frame = frames_[currentFrame_];
    painter->drawPixmap(pos(), image_, frame.rect);
}

QRectF Sprite::boundingRect() const
{
    const qreal penWidth = 1;
    const QRect& rect = frames_[currentFrame_].rect;
    return QRectF(pos().x() - penWidth, pos().y() - penWidth, rect.width() + penWidth, rect.height() + penWidth);
}

void Sprite::nextFrame()
{
    ++currentFrame_;
    currentFrame_ %= frames_.size();
    
    timer_.setInterval(frames_[currentFrame_].duration);

    update(boundingRect());
}

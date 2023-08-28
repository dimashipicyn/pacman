#pragma once

#include <QGraphicsItem>
#include <QTimer>
#include <QPixmap>
#include <QPainter>

class Tile : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    struct Frame
    {
        QRect rect;
        int duration;
    };

    Tile(QPixmap image, QVector<Frame> frames, QObject* parent = nullptr);

    void playAnimation(bool play);
    void repeatAnimation(bool repeat);

    bool isPlayedAnimation() const;
    bool isEndAnimation() const;
    bool isRepeated() const;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    QRectF boundingRect() const override;

private:
    void nextFrame();

    QPixmap image_;
    QTimer timer_;
    QVector<Frame> frames_;
    int currentFrame_ = 0;
    bool playedAnimation_ = true;
    bool repeatedAnimation_ = true;
};
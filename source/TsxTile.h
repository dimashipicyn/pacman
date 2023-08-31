#pragma once

#include <QString>
#include <QHash>
#include <QVector>

namespace Tiled
{

struct TsxTileAnimationFrame
{
    int tileId;
    int duration;
};

struct TsxTile
{
    int id;
    QString type;

    QString getProperty(const QString& name) const;
    void setProperty(const QString& name, const QString& value);

    void addAnimationFrame(TsxTileAnimationFrame frame);
    TsxTileAnimationFrame getAnimationFrame(int index) const;
    int animationFrames() const;

private:
    QHash<QString, QString> properties;
    QVector<TsxTileAnimationFrame> animation;
};

}
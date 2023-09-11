#pragma once

#include <Tiled/TmxObject.h>

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

    QString getProperty(const QString& key) const;
    void setProperty(const QString& key, const QString& value);

    void addAnimationFrame(TsxTileAnimationFrame frame);
    TsxTileAnimationFrame getAnimationFrame(int index) const;
    int animationFrames() const;

    void addObject(TmxObject tmxObject);
    TmxObject getObject(const QString& type) const;

private:
    QHash<QString, QString> properties;
    QHash<QString, TmxObject> objects;
    QVector<TsxTileAnimationFrame> animation;
};

}
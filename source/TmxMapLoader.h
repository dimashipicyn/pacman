#pragma once

#include "TmxObject.h"
#include "TmxTileLayer.h"
#include "TmxObjectLayer.h"
#include "TmxTileset.h"

#include <QString>
#include <QVector>
#include <cassert>

class QXmlStreamReader;
class QXmlStreamAttributes;
class TmxObject;

namespace Tiled
{

class TmxMapLoader
{
public:
    bool load(const QString& tmxMapPath);

    TmxTileLayer getTileLayer(const QString& name) const;
    TmxObjectLayer getObjectLayer(const QString& name) const;
    TmxTileset getTileset(const QString& name) const;

    int width() const;
    int height() const;
    int tileWidth() const;
    int tileHeight() const;

private:
    void parseMap(const QXmlStreamAttributes& attrs);
    void parseTilesets(const QXmlStreamAttributes& attrs);
    void parseTileLayer(QXmlStreamReader& reader);
    void parseObjectLayer(QXmlStreamReader& reader);
    void parseObjectProperties(QXmlStreamReader& reader, TmxObject& dest);

private:
    QHash<QString, TmxTileLayer> tileLayers_;
    QHash<QString, TmxObjectLayer> objectLayers_;
    QHash<QString, TmxTileset> tilesets_;

    int width_ = 0;
    int height_ = 0;
    int tileWidth_ = 0;
    int tileHeight_ = 0;
};

}

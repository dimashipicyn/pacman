#include "TmxMapLoader.h"

#include "Tile.h"
#include "TsxTileset.h"

#include <QFile>
#include <QFileInfo>
#include <QXmlStreamReader>
#include <QDebug>

namespace Tiled
{

bool TmxMapLoader::load(const QString& tmx_path)
{
    QFile file(tmx_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    QXmlStreamReader reader(&file);
    if (reader.hasError())
    {
        return false;
    }

    while (!reader.atEnd())
    {
        if (reader.name() == "tileset" && !reader.isEndElement())
        {
            parseTilesets(reader.attributes());
        }
        else if (reader.name() == "objectgroup" && !reader.isEndElement())
        {
            parseObjectLayer(reader);
        }
        else if (reader.name() == "layer" && !reader.isEndElement())
        {
            parseTileLayer(reader);
        }
        else if (reader.name() == "map" && !reader.isEndElement())
        {
            parseMap(reader.attributes());
        }

        reader.readNextStartElement();
    }

    return true;
}

TmxTileLayer TmxMapLoader::getTileLayer(const QString& name) const
{
    if (tileLayers_.contains(name))
    {
        return tileLayers_.value(name);
    }
    qWarning() << "Tile layer '" << name << "' does not exists!";
    return TmxTileLayer();
}

TmxObjectLayer TmxMapLoader::getObjectLayer(const QString& name) const
{
    if (objectLayers_.contains(name))
    {
        return objectLayers_.value(name);
    }
    qWarning() << "Object layer '" << name << "' does not exists!";
    return TmxObjectLayer();
}

QVector<TmxTileset> TmxMapLoader::getTilesets() const
{
    return tilesets_;
}

int TmxMapLoader::width() const
{
    return width_;
}

int TmxMapLoader::height() const
{
    return height_;
}

int TmxMapLoader::tileWidth() const
{
    return tileWidth_;
}

int TmxMapLoader::tileHeight() const
{
    return tileHeight_;
}

void TmxMapLoader::parseMap(const QXmlStreamAttributes& attrs)
{
    width_ = attrs.value("width").toInt();
    height_ = attrs.value("height").toInt();
    tileWidth_ = attrs.value("tilewidth").toInt();
    tileHeight_ = attrs.value("tileheight").toInt();
}

void TmxMapLoader::parseTilesets(const QXmlStreamAttributes& attrs)
{
    TmxTileset tileset;
    tileset.firstGid = attrs.value("firstgid").toInt();
    tileset.source = attrs.value("source").toString();

    tilesets_.push_back(std::move(tileset));
}

void TmxMapLoader::parseTileLayer(QXmlStreamReader& reader)
{
    QXmlStreamAttributes attrs = reader.attributes();

    TmxTileLayer tileLayer;
    tileLayer.id = attrs.value("id").toInt();
    tileLayer.name = attrs.value("name").toString();
    tileLayer.width = attrs.value("width").toInt();
    tileLayer.height = attrs.value("height").toInt();

    reader.readNextStartElement();

    if (reader.name() == "data")
    {
        QString value = reader.readElementText();
        for (QString& elem : value.split(","))
        {
            tileLayer.data.push_back(elem.toInt());
        }

        reader.readNextStartElement();
    }
    QString name = tileLayer.name;
    tileLayers_.insert(name, std::move(tileLayer));
}

void TmxMapLoader::parseObjectLayer(QXmlStreamReader& reader)
{
    QXmlStreamAttributes attrs = reader.attributes();

    TmxObjectLayer objectLayer;
    objectLayer.id = attrs.value("id").toInt();
    objectLayer.name = attrs.value("name").toString();

    reader.readNextStartElement();

    while (reader.name() == "object")
    {
        if (reader.isEndElement())
        {
            reader.readNextStartElement();
            continue;
        }

        QXmlStreamAttributes attrs = reader.attributes();

        TmxObject obj;
        obj.id = attrs.value("id").toInt();
        obj.name = attrs.value("name").toString();
        obj.type = attrs.value("type").toString();
        obj.id = attrs.value("gid").toInt();

        float x = attrs.value("x").toFloat();
        float y = attrs.value("y").toFloat();
        obj.position = QPointF(x, y);

        float width = attrs.value("width").toFloat();
        float height = attrs.value("height").toFloat();
        obj.size = QSize(width, height);

        reader.readNextStartElement();
        if (reader.name() == "properties")
        {
            parseObjectProperties(reader, obj);
            reader.readNextStartElement();
        }

        objectLayer.objects.insert(obj.name, obj);
    }

    QString name = objectLayer.name;
    objectLayers_.insert(name, std::move(objectLayer));
}

void TmxMapLoader::parseObjectProperties(QXmlStreamReader& reader, TmxObject& dest)
{
    reader.readNextStartElement();

    while (reader.name() == "property")
    {
        if (reader.isStartElement())
        {
            QXmlStreamAttributes attrs = reader.attributes();
            QString name = attrs.value("name").toString();
            QString value = attrs.value("value").toString();
            dest.addProperty(name, value);
        }

        reader.readNextStartElement();
    }
}

}

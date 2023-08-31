#include "TsxTileset.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamAttributes>
#include <QDebug>

namespace Tiled
{

bool TsxTileset::load(const QString& tsx_path)
{
    QFile file(tsx_path);
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
            parseTileset(reader.attributes());
        }
        else if (reader.name() == "image" && !reader.isEndElement())
        {
            parseImage(reader.attributes());
        }
        else if (reader.name() == "tile" && !reader.isEndElement())
        {
            parseTile(reader);
        }

        reader.readNextStartElement();
    }

    if (!image_.load(":assets/" + source_))
    {
        return false;
    }

    file.close();

    return true;
}

int TsxTileset::tileCount() const
{
    return tileCount_;
}

int TsxTileset::tileWidth() const
{
    return tileWidth_;
}

int TsxTileset::tileHeight() const
{
    return tileHeight_;
}

TsxTile TsxTileset::getTile(const QString& type)
{
    if (tiles_.contains(type))
    {
        return tiles_.value(type);
    }
    qWarning() << "Tile with type '" << type << "' does not exist!";
    return TsxTile();
}

TsxTile TsxTileset::getTile(int id)
{
    if (ids_.contains(id))
    {
        return getTile(ids_.value(id));
    }

    TsxTile tile;
    tile.id = id;
    return tile;
}

TsxTilesetFrame TsxTileset::getFrameFromId(int id)
{
    return TsxTilesetFrame();
}

void TsxTileset::parseTileset(const QXmlStreamAttributes& attrs)
{
    tileWidth_ = attrs.value("tilewidth").toInt();
    tileHeight_ = attrs.value("tileheight").toInt();
    margin_ = attrs.value("margin").toInt();
    spacing_ = attrs.value("spacing").toInt();
    tileCount_ = attrs.value("tilecount").toInt();
    columns_ = attrs.value("columns").toInt();
}

void TsxTileset::parseImage(const QXmlStreamAttributes& attrs)
{
    source_ = attrs.value("source").toString();
}

void TsxTileset::parseTile(QXmlStreamReader& reader)
{
    QXmlStreamAttributes attrs = reader.attributes();

    TsxTile tile;

    tile.id = attrs.value("id").toInt();
    tile.type = attrs.value("type").toString();

    reader.readNextStartElement();
    if (reader.name() == "animation")
    {
        parseTileAnimation(reader, tile);
    }

    if (reader.name() == "properties")
    {
        parseTileProperties(reader, tile);
    }

    QString key = tile.type;
    ids_.insert(tile.id, key);
    tiles_.insert(key, std::move(tile));
}

void TsxTileset::parseTileAnimation(QXmlStreamReader& reader, TsxTile& tile)
{
    reader.readNextStartElement();
    while (reader.name() == "frame")
    {
        if (reader.isStartElement())
        {
            QXmlStreamAttributes attrs = reader.attributes();

            TsxTileAnimationFrame frame;
            frame.tileId = attrs.value("tileid").toInt();
            frame.duration = attrs.value("duration").toInt();

            tile.addAnimationFrame(std::move(frame));
        }
        reader.readNextStartElement();
    }
}

void TsxTileset::parseTileProperties(QXmlStreamReader& reader, TsxTile& tile)
{
    reader.readNextStartElement();
    while (reader.name() == "property" && reader.isStartElement())
    {
        QXmlStreamAttributes attrs = reader.attributes();

        QString name = attrs.value("name").toString();
        QString value = attrs.value("value").toString();

        tile.setProperty(name, value);
        reader.readNextStartElement();
    }
}

QRect TsxTileset::getTileRect(int id)
{
    assert(id < tileCount_);

    int px = id % columns_;
    int py = id / columns_;
    int x = px * tileWidth_ + margin_ + px * spacing_;
    int y = py * tileHeight_ + margin_ + py * spacing_;

    return QRect(x, y, tileWidth_, tileHeight_);
}

}
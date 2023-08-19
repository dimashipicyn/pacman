#include "SpriteSheet.h"

#include "Sprite.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QDebug>

SpriteSheet::SpriteSheet(QObject* parent)
    : QObject(parent)
{

}

bool SpriteSheet::load(const QString& tsx_path)
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
            parse_tileset(reader.attributes());
        }
        else if (reader.name() == "image" && !reader.isEndElement())
        {
            parse_image(reader.attributes());
        }
        else if (reader.name() == "tile" && !reader.isEndElement())
        {
            parse_tile(reader);
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

Sprite* SpriteSheet::getSprite(const QString& type)
{
    Tile tile = getTileByType(type);
    QVector<Sprite::Frame> frames;
    if (tile.animation.empty())
    {
        frames.push_back({ getTileRect(tile.id), 0});
    }
    else
    {
        for (const AnimationFrame& frame : tile.animation)
        {
            frames.push_back({ getTileRect(frame.tileId), frame.duration });
        }
    }

    return new Sprite(image_, std::move(frames), this);
}

Sprite* SpriteSheet::getSprite(int id)
{
    if (ids_.contains(id))
    {
        return getSprite(ids_.value(id));
    }

    return new Sprite(image_, { { getTileRect(id), 0 } }, this);
}

int SpriteSheet::sprites() const
{
    return tilecount_;
}

void SpriteSheet::parse_tileset(const QXmlStreamAttributes& attrs)
{
    tilewidth_ = attrs.value("tilewidth").toInt();
    tileheight_ = attrs.value("tileheight").toInt();
    margin_ = attrs.value("margin").toInt();
    spacing_ = attrs.value("spacing").toInt();
    tilecount_ = attrs.value("tilecount").toInt();
    columns_ = attrs.value("columns").toInt();
}

void SpriteSheet::parse_image(const QXmlStreamAttributes& attrs)
{
    source_ = attrs.value("source").toString();
}

void SpriteSheet::parse_tile(QXmlStreamReader& reader)
{
    QXmlStreamAttributes attrs = reader.attributes();

    Tile tile;

    tile.id = attrs.value("id").toInt();
    tile.type = attrs.value("type").toString();

    reader.readNextStartElement();
    if (reader.name() == "animation")
    {
        reader.readNextStartElement();
        while (reader.name() == "frame")
        {
            if (reader.isStartElement())
            {
                QXmlStreamAttributes attrs = reader.attributes();

                AnimationFrame frame;
                frame.tileId = attrs.value("tileid").toInt();
                frame.duration = attrs.value("duration").toInt();

                tile.animation.push_back(std::move(frame));
            }
            reader.readNextStartElement();
        }
    }

    if (reader.name() == "properties")
    {
        reader.readNextStartElement();
        while (reader.name() == "property" && reader.isStartElement())
        {
            QXmlStreamAttributes attrs = reader.attributes();

            QString name = attrs.value("name").toString();
            QString value = attrs.value("value").toString();

            tile.properties.insert(name, value);
            reader.readNextStartElement();
        }
    }

    QString key = tile.type;
    ids_.insert(tile.id, key);
    tiles_.insert(key, std::move(tile));
}

QRect SpriteSheet::getTileRect(int id)
{
    assert(id < tilecount_);

    int px = id % columns_;
    int py = id / columns_;
    int x = px * tilewidth_ + margin_ + px * spacing_;
    int y = py * tileheight_ + margin_ + py * spacing_;

    return QRect(x, y, tilewidth_, tileheight_);
}

SpriteSheet::Tile SpriteSheet::getTileByType(const QString& type)
{
    if (auto it = tiles_.find(type); it != tiles_.end())
    {
        return *it;
    }
    assert(false);
    return Tile {};
}


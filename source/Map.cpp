#include "Map.h"

#include "SpriteSheet.h"
#include "Sprite.h"

#include <QFile>
#include <QXmlStreamReader>

bool Map::load(const QString& tmx_path)
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
            parse_tileset(reader.attributes());
        }
        else if (reader.name() == "image" && !reader.isEndElement())
        {
            //parse_image(reader.attributes());
        }
        else if (reader.name() == "layer" && !reader.isEndElement())
        {
            parse_layer(reader);
        }

        reader.readNextStartElement();
    }

    for (Tileset& ts : tilesets_)
    {
        ts.sheet = new SpriteSheet(this);
        if (!ts.sheet->load(":/assets/" + ts.source))
        {
            return false;
        }
    }

    for (int id : map_)
    {
        if (auto sit = sprites_.find(id); sit == sprites_.end())
        {
            auto tit = std::find_if(tilesets_.begin(), tilesets_.end(), [id](const Tileset& ts)
            {
                return ts.first_gid <= id && (ts.first_gid + ts.sheet->sprites()) > id;
            });
            if (tit != tilesets_.end())
            {
                sprites_.insert(id, tit->sheet->getSprite(id - tit->first_gid));
            }
        }
    }

    return true;
}

void Map::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    for (int y = 0; y < height_; ++y)
    {
        for (int x = 0; x < width_; ++x)
        {
            Sprite* s = sprites_.value(map_[y * width_ + x]);
            s->setPos(QPointF(x * 8, y * 8));
            s->paint(painter, option, widget);
        }
    }
}

QRectF Map::boundingRect() const
{
    return QRectF(0, 0, width_ * 8, height_ * 8);
}

void Map::parse_tileset(const QXmlStreamAttributes& attrs)
{
    Tileset tileset;
    tileset.first_gid = attrs.value("firstgid").toInt();
    tileset.source = attrs.value("source").toString();

    tilesets_.push_back(std::move(tileset));
}

void Map::parse_layer(QXmlStreamReader& reader)
{
    QXmlStreamAttributes attrs = reader.attributes();

    width_ = attrs.value("width").toInt();
    height_ = attrs.value("height").toInt();

    reader.readNextStartElement();

    if (reader.name() == "data")
    {
        QString value = reader.readElementText();
        for (QString& elem : value.split(","))
        {
            map_.push_back(elem.toInt());
        }

        reader.readNextStartElement();
    }
}

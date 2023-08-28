#include "TmxMap.h"

#include "TsxTileset.h"
#include "Tile.h"

#include <QFile>
#include <QXmlStreamReader>

TmxMap::TmxMap(QObject* parent)
    : QObject(parent)
    , QGraphicsItem()
{
}

bool TmxMap::load(const QString& tmx_path)
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
        else if (reader.name() == "objectgroup" && !reader.isEndElement())
        {
            parse_objects(reader);
        }
        else if (reader.name() == "layer" && !reader.isEndElement())
        {
            parse_layer(reader);
        }

        reader.readNextStartElement();
    }

    for (Tileset& ts : tilesets_)
    {
        ts.tileset = new TsxTileset(this);
        if (!ts.tileset->load(":/assets/" + ts.source))
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
                return ts.first_gid <= id && (ts.first_gid + ts.tileset->tileCount()) > id;
            });
            if (tit != tilesets_.end())
            {
                sprites_.insert(id, tit->tileset->getTile(id - tit->first_gid));
            }
        }
    }

    update();

    return true;
}

TmxObject TmxMap::getObject(const QString& name) const
{
    if (auto it = objects_.find(name); it != objects_.end())
    {
        return *it;
    }
    qWarning() << "Object with name: '" << name << "' not found!";
    return TmxObject();
}

void TmxMap::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    for (int y = 0; y < height_; ++y)
    {
        for (int x = 0; x < width_; ++x)
        {
            Tile* s = sprites_.value(map_[y * width_ + x]);
            s->setPos(QPointF(x * 8, y * 8));
            s->paint(painter, option, widget);
        }
    }
}

QRectF TmxMap::boundingRect() const
{
    return QRectF(0, 0, width_ * 8, height_ * 8);
}

void TmxMap::parse_tileset(const QXmlStreamAttributes& attrs)
{
    Tileset tileset;
    tileset.first_gid = attrs.value("firstgid").toInt();
    tileset.source = attrs.value("source").toString();

    tilesets_.push_back(std::move(tileset));
}

void TmxMap::parse_layer(QXmlStreamReader& reader)
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

void TmxMap::parse_objects(QXmlStreamReader& reader)
{
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
        obj.id = attrs.value("gid").toInt();

        int x = attrs.value("x").toInt();
        int y = attrs.value("y").toInt();
        int width = attrs.value("width").toInt();
        int height = attrs.value("height").toInt();
        obj.rect = QRect(x, y, width, height);

        reader.readNextStartElement();
        if (reader.name() == "properties")
        {
            parse_properties(reader, obj);
            reader.readNextStartElement();
        }

        objects_.insert(obj.name, obj);
    }
    qDebug() << "name " << reader.name();
}

void TmxMap::parse_properties(QXmlStreamReader& reader, TmxObject& dest)
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

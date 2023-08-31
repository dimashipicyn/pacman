#include "TmxMap.h"

#include "TsxTileset.h"
#include "Tile.h"

#include <QFile>
#include <QXmlStreamReader>

TmxMap::TmxMap(QObject* parent)
    : QObject(parent)
    , QGraphicsItemGroup()
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
        else if (reader.name() == "map" && !reader.isEndElement())
        {
            parse_map(reader.attributes());
        }

        reader.readNextStartElement();
    }

    create_tilesets();
    create_maps();

    return true;
}

TmxObject TmxMap::getObject(const QString& name) const
{
    for (auto& object : objects_)
    {
        if (auto it = object.objects.find(name); it != object.objects.end())
        {
            return *it;
        }
    }
    qWarning() << "Object with name: '" << name << "' not found!";
    return TmxObject();
}

void TmxMap::parse_map(const QXmlStreamAttributes& attrs)
{
    width_ = attrs.value("width").toInt();
    height_ = attrs.value("height").toInt();
    tilewidth_ = attrs.value("tilewidth").toInt();
    tileheight_ = attrs.value("tileheight").toInt();
}

void TmxMap::parse_tileset(const QXmlStreamAttributes& attrs)
{
    Tileset tileset;
    tileset.firstGid = attrs.value("firstgid").toInt();
    tileset.source = attrs.value("source").toString();

    tilesets_.push_back(std::move(tileset));
}

void TmxMap::parse_layer(QXmlStreamReader& reader)
{
    QXmlStreamAttributes attrs = reader.attributes();

    MapLayer mapLayer;
    mapLayer.id = attrs.value("id").toInt();
    mapLayer.name = attrs.value("name").toString();
    mapLayer.width = attrs.value("width").toInt();
    mapLayer.height = attrs.value("height").toInt();

    reader.readNextStartElement();

    if (reader.name() == "data")
    {
        QString value = reader.readElementText();
        for (QString& elem : value.split(","))
        {
            mapLayer.data.push_back(elem.toInt());
        }

        reader.readNextStartElement();
    }

    mapLayers_.push_back(std::move(mapLayer));
}

void TmxMap::parse_objects(QXmlStreamReader& reader)
{
    QXmlStreamAttributes attrs = reader.attributes();

    ObjectsLayer objectsLayer;
    objectsLayer.id = attrs.value("id").toInt();
    objectsLayer.name = attrs.value("name").toString();

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

        float x = attrs.value("x").toFloat();
        float y = attrs.value("y").toFloat();
        obj.position = QPointF(x, y);

        float width = attrs.value("width").toFloat();
        float height = attrs.value("height").toFloat();
        obj.size = QSize(width, height);

        reader.readNextStartElement();
        if (reader.name() == "properties")
        {
            parse_properties(reader, obj);
            reader.readNextStartElement();
        }

        objectsLayer.objects.insert(obj.name, obj);
    }

    objects_.push_back(std::move(objectsLayer));
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

void TmxMap::create_tilesets()
{
    for (Tileset& ts : tilesets_)
    {
        auto tsxTileset = new TsxTileset(this);
        if (tsxTileset->load(":/assets/" + ts.source))
        {
            ts.tileset = tsxTileset;
        }
        else
        {
            qWarning() << "Tileset '" << ts.source << "' not loaded!";
            delete tsxTileset;
        }
    }

    // erase not loaded tilesets
    tilesets_.erase(std::remove_if(tilesets_.begin(), tilesets_.end(), [](const Tileset& ts)
                        { return ts.tileset == nullptr; }),
        tilesets_.end());
}

void TmxMap::create_maps()
{
    for (const MapLayer& mapLayer : mapLayers_)
    {
        create_map(mapLayer);
    }
}

void TmxMap::create_map(const MapLayer& mapLayer)
{
    int width = mapLayer.width;
    int height = mapLayer.height;

    for (int y = 0; y < height_; ++y)
    {
        for (int x = 0; x < width_; ++x)
        {
            int id = mapLayer.data[y * width_ + x];

            Tileset tileset = getTilesetFromTileId(id);

            QPointF tile_position(x * tilewidth_, y * tileheight_);

            int idWithOffset = id - tileset.firstGid;
            Tile* tile = tileset.tileset->getTile(idWithOffset);
            tile->setPos(tile_position);
            tile->setSize(QSize(tilewidth_, tileheight_));

            addToGroup(tile);
        }
    }
}

TmxMap::Tileset TmxMap::getTilesetFromTileId(int tileId)
{
    // find tileset for ids range
    auto tilesetFinder = [tileId](const Tileset& ts)
    {
        bool greaterThanFirstGid = ts.firstGid <= tileId;
        bool lessThanTileCount = (ts.firstGid + ts.tileset->tileCount()) > tileId;
        return greaterThanFirstGid && lessThanTileCount;
    };

    auto tileset = std::find_if(tilesets_.begin(), tilesets_.end(), std::move(tilesetFinder));
    if (tileset != tilesets_.end())
    {
        return *tileset;
    }

    qWarning() << "Tileset with tile id: " << tileId << " not found!";
    assert(false);
    return Tileset();
}

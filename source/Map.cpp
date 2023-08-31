#include "Map.h"

#include "TmxTileset.h"
#include "TsxTile.h"
#include "TsxTileset.h"
#include "Tile.h"

#include <QFile>
#include <QDebug>

Map::Map(Tiled::TmxMapLoader& mapLoader, QObject* parent)
    : QObject(parent)
    , QGraphicsItemGroup()
{
    tileLayer_ = mapLoader.getTileLayer("map");
    loadTilesets(mapLoader);
    createMap(mapLoader);
}

Map::~Map()
{
    for (auto ts : tilesets_)
    {
        delete ts;
    }
}

void Map::loadTilesets(Tiled::TmxMapLoader& mapLoader)
{
    for (const Tiled::TmxTileset& tmxTileset : mapLoader.getTilesets())
    {
        Tiled::TsxTileset* tsxTileset = new Tiled::TsxTileset;
        QString filename = ":/assets/" + tmxTileset.source;
        if (tsxTileset->load(filename, tmxTileset.firstGid))
        {
            tilesets_.push_back(tsxTileset);
        }
        else
        {
            delete tsxTileset;
        }
    }
}

void Map::createMap(Tiled::TmxMapLoader& mapLoader)
{
    int tileWidth = mapLoader.tileWidth();
    int tileHeight = mapLoader.tileHeight();
    int width = tileLayer_.width;
    int height = tileLayer_.height;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int id = tileLayer_.getTileId(x, y);

            Tiled::TsxTileset* tileset = getTilesetFromTileId(id);
            Tiled::TsxTile tsxTile = tileset->getTile(id);

            QPointF tile_position(x * tileWidth, y * tileHeight);

            Tile* tile = new Tile(tileset, tsxTile, this);
            tile->setPos(tile_position);
            tile->setSize(QSize(tileWidth, tileHeight));

            addToGroup(tile);
        }
    }
}

Tiled::TsxTileset* Map::getTilesetFromTileId(int tileId)
{
    // find tileset for ids range
    auto tilesetFinder = [tileId](const Tiled::TsxTileset* ts)
    {
        bool greaterThanFirstGid = ts->firstGid() <= tileId;
        bool lessThanTileCount = (ts->firstGid() + ts->tileCount()) > tileId;
        return greaterThanFirstGid && lessThanTileCount;
    };

    auto tileset = std::find_if(tilesets_.begin(), tilesets_.end(), std::move(tilesetFinder));
    if (tileset != tilesets_.end())
    {
        return *tileset;
    }

    qWarning() << "Tileset with tile id: " << tileId << " not found!";
    assert(false);
    return nullptr;
}

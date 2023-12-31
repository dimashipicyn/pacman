#pragma once

#include "Tiled/TmxMapLoader.h"
#include "Tiled/TmxTileLayer.h"
#include "Tiled/TsxTileset.h"

#include <QObject>
#include <QGraphicsItemGroup>
#include <QVector>

class Map : public QObject, public QGraphicsItemGroup
{
public:
    Map(Tiled::TmxMapLoader& mapLoader, QObject* parent = nullptr);
    ~Map();
    
private:
    void loadTilesets(Tiled::TmxMapLoader& mapLoader);
    void createMap(Tiled::TmxMapLoader& mapLoader);

    Tiled::TsxTileset* getTilesetFromTileId(int tileId);

private:
    Tiled::TmxTileLayer tileLayer_;
    QVector<Tiled::TsxTileset*> tilesets_;
};
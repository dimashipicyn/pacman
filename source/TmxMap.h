#pragma once

#include "TmxObject.h"

#include <QObject>
#include <QGraphicsItemGroup>
#include <QString>
#include <QVector>
#include <QHash>
#include <QRect>
#include <QXmlStreamReader>
#include <QXmlStreamAttributes>
#include <QDebug>

class TsxTileset;
class Tile;

class TmxMap : public QObject, public QGraphicsItemGroup
{
public:
    TmxMap(QObject* parent = nullptr);
    bool load(const QString& tmx_path);

    TmxObject getObject(const QString& name) const;

private:
    void parse_map(const QXmlStreamAttributes& attrs);
    void parse_tileset(const QXmlStreamAttributes& attrs);
    void parse_layer(QXmlStreamReader& reader);
    void parse_objects(QXmlStreamReader& reader);
    void parse_properties(QXmlStreamReader& reader, TmxObject& dest);

    struct Tileset
    {
        TsxTileset* tileset = nullptr;
        QString source;
        int firstGid = 0;
    };

    struct MapLayer
    {
        int id = 0;
        int width = 0;
        int height = 0;
        QString name;
        QVector<int> data;
    };

    struct ObjectsLayer
    {
        int id = 0;
        QString name;
        QHash<QString, TmxObject> objects;
    };

    void create_tilesets();
    void create_maps();
    void create_map(const MapLayer& mapLayer);

    Tileset getTilesetFromTileId(int tileId);

private:
    QVector<Tileset> tilesets_;
    QVector<MapLayer> mapLayers_;
    QVector<ObjectsLayer> objects_;
    int width_ = 0;
    int height_ = 0;
    int tilewidth_ = 0;
    int tileheight_ = 0;
};
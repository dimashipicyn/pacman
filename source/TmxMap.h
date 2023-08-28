#pragma once

#include "TmxObject.h"

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsItem>
#include <QString>
#include <QVector>
#include <QHash>
#include <QRect>
#include <QXmlStreamReader>
#include <QXmlStreamAttributes>
#include <QDebug>

class TsxTileset;
class Tile;

class TmxMap : public QObject, public QGraphicsItem
{
public:
    TmxMap(QObject* parent = nullptr);

    bool load(const QString& tmx_path);

    TmxObject getObject(const QString& name) const;

private:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    QRectF boundingRect() const override;

    void parse_map(const QXmlStreamAttributes& attrs);
    void parse_tileset(const QXmlStreamAttributes& attrs);
    void parse_layer(QXmlStreamReader& reader);
    void parse_objects(QXmlStreamReader& reader);
    void parse_properties(QXmlStreamReader& reader, TmxObject& dest);

    struct Tileset
    {
        TsxTileset* tileset = nullptr;
        QString source;
        int first_gid;
    };

private:
    QVector<int> map_;
    QVector<Tileset> tilesets_;
    QHash<int, Tile*> sprites_;
    QHash<QString, TmxObject> objects_;
    int width_ = 0;
    int height_ = 0;
    int tilewidth_ = 0;
    int tileheight_ = 0;
};
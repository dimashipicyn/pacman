#pragma once

#include <QObject>
#include <QPixmap>
#include <QXmlStreamAttributes>
#include <QXmlStreamReader>
#include <QString>
#include <QVector>
#include <QHash>

class Tile;

class TsxTileset : public QObject
{
	Q_OBJECT
public:
    TsxTileset(QObject* parent = nullptr);

    bool load(const QString& tsx_path);
    
    int tileCount() const;
    int tileWidth() const;
    int tileHeight() const;
    Tile* getTile(const QString& type);
    Tile* getTile(int id);

private:
    struct AnimationFrame
    {
        int tileId;
        int duration;
    };

    struct TilesetTile
    {
        int id;
        QString type;
        QHash<QString, QString> properties;
        QVector<AnimationFrame> animation;
    };

    void parse_tileset(const QXmlStreamAttributes& attrs);
    void parse_image(const QXmlStreamAttributes& attrs);
    void parse_tile(QXmlStreamReader& reader);

    QRect getTileRect(int id);
    TilesetTile getTileByType(const QString& type);

private:
    QPixmap image_;
    QHash<QString, TilesetTile> tiles_;
    QHash<int, QString> ids_;
    QString source_;
    int tilewidth_ = 0;
    int tileheight_ = 0;
    int spacing_ = 0;
    int margin_ = 0;
    int tilecount_ = 0;
    int columns_ = 0;
};
#pragma once

#include "TsxTile.h"

#include <QPixmap>
#include <QString>
#include <QVector>
#include <QHash>

class QXmlStreamReader;
class QXmlStreamAttributes;

namespace Tiled
{

struct TsxTilesetFrame
{
    QPixmap image;
    QRect srcRect;
};

class TsxTileset
{
public:
    bool load(const QString& tsx_path, int firstGid = 0);

    int tileCount() const;
    int tileWidth() const;
    int tileHeight() const;
    int firstGid() const;

    TsxTile getTile(const QString& type);
    TsxTile getTile(int id);

    TsxTilesetFrame getFrameFromId(int id);

private:
    void parseTileset(const QXmlStreamAttributes& attrs);
    void parseImage(const QXmlStreamAttributes& attrs);
    void parseTile(QXmlStreamReader& reader);
    void parseTileAnimation(QXmlStreamReader& reader, TsxTile& tile);
    void parseTileProperties(QXmlStreamReader& reader, TsxTile& tile);

    QRect getTileRect(int id);

private:
    QPixmap image_;
    QHash<QString, TsxTile> tiles_;
    QHash<int, QString> ids_;
    QString source_;
    int tileWidth_ = 0;
    int tileHeight_ = 0;
    int spacing_ = 0;
    int margin_ = 0;
    int tileCount_ = 0;
    int columns_ = 0;
    int firstGid_ = 0;
};

}
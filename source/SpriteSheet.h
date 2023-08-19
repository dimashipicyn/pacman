#include <QObject>
#include <QPixmap>
#include <QXmlStreamAttributes>
#include <QXmlStreamReader>
#include <QString>
#include <QVector>
#include <QHash>

class Sprite;

class SpriteSheet : public QObject
{
	Q_OBJECT
public:
    SpriteSheet(QObject* parent = nullptr);

    bool load(const QString& tsx_path);
    
    int sprites() const;
    Sprite* getSprite(const QString& type);
    Sprite* getSprite(int id);

private:
    struct AnimationFrame
    {
        int tileId;
        int duration;
    };

    struct Tile
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
    Tile getTileByType(const QString& type);

private:
    QPixmap image_;
    QHash<QString, Tile> tiles_;
    QHash<int, QString> ids_;
    QString source_;
    int tilewidth_ = 0;
    int tileheight_ = 0;
    int spacing_ = 0;
    int margin_ = 0;
    int tilecount_ = 0;
    int columns_ = 0;
};
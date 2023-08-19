#include <QObject>
#include <QGraphicsItem>
#include <QString>
#include <QVector>
#include <QHash>
#include <QRect>
#include <QXmlStreamReader>
#include <QXmlStreamAttributes>

class SpriteSheet;
class Sprite;

class Map : public QObject, public QGraphicsItem
{
public:
    bool load(const QString& tmx_path);

    struct Object
    {
        QString name;
        int id;
        int gid;
        QRect rect;
    };

private:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    QRectF boundingRect() const override;

    void parse_tileset(const QXmlStreamAttributes& attrs);
    void parse_layer(QXmlStreamReader& reader);

    struct Tileset
    {
        SpriteSheet* sheet = nullptr;
        QString source;
        int first_gid;
    };

private:
    QVector<int> map_;
    QVector<Tileset> tilesets_;
    QHash<int, Sprite*> sprites_;
    QHash<QString, Object> objects_;
    int width_ = 0;
    int height_ = 0;
};
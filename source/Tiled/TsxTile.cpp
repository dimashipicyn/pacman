#include "TsxTile.h"

#include <QDebug>

namespace Tiled
{

QString TsxTile::getProperty(const QString& key) const
{
    if (properties.contains(key))
    {
        return properties.value(key);
    }
    qWarning() << "Property: '" << key << "' in TsxTile doesnt exist!";
    return QString();
}

void TsxTile::setProperty(const QString& key, const QString& value)
{
    properties.insert(key, value);
}

void TsxTile::addAnimationFrame(TsxTileAnimationFrame frame)
{
    animation.push_back(std::move(frame));
}

TsxTileAnimationFrame TsxTile::getAnimationFrame(int index) const
{
    return animation.at(index);
}

int TsxTile::animationFrames() const
{
    return animation.size();
}

void TsxTile::addObject(TmxObject tmxObject)
{
    objects.insert(tmxObject.type, tmxObject);
}

TmxObject TsxTile::getObject(const QString& type) const
{
    if (objects.contains(type))
    {
        return objects.value(type);
    }
    qWarning() << "Object with type: '" << type << "' in TsxTile doesnt exist!";
    return TmxObject();
}

}

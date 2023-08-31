#include "TsxTile.h"

namespace Tiled
{

QString TsxTile::getProperty(const QString& name) const
{
    return properties.contains(name) ? properties.value(name) : QString();
}

void TsxTile::setProperty(const QString& name, const QString& value)
{
    properties.insert(name, value);
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

}

#include "TmxObjectLayer.h"

#include <QDebug>

namespace Tiled
{

TmxObject TmxObjectLayer::getObject(const QString& name)
{
    if (objects.contains(name))
    {
        return objects.value(name);
    }
    qWarning() << "Object '" << name << "' does not exists!";
    return TmxObject();
}

}


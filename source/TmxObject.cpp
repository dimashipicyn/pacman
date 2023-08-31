#include "TmxObject.h"

#include <QDebug>

namespace Tiled
{

QString TmxObject::getProperty(const QString& name) const
{
    if (auto it = properties_.find(name); it != properties_.end())
    {
        return *it;
    }
    qWarning() << "Property '" << name << "' in object '" << this->name << "' not found!";
    return QString();
}

void TmxObject::addProperty(const QString& name, const QString& value)
{
    properties_.insert(name, value);
}

}

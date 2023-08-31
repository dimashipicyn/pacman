#pragma once

#include "TmxObject.h"

#include <QString>
#include <QHash>

namespace Tiled
{

struct TmxObjectLayer
{
    int id = 0;
    QString name;
    QHash<QString, TmxObject> objects;

    TmxObject getObject(const QString& name);
};

}

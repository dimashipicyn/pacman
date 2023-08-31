#pragma once

#include <QString>
#include <QPointF>
#include <QSize>
#include <QHash>

namespace Tiled
{

class TmxObject
{
public:
    QString name;
    int id;
    int gid;
    QPointF position;
    QSize size;

    QString getProperty(const QString& name) const;
    void addProperty(const QString& name, const QString& value);

private:
    QHash<QString, QString> properties_;
};

}

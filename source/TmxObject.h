#pragma once

#include <QString>
#include <QRect>
#include <QHash>

class TmxObject
{
public:
    QString name;
    int id;
    int gid;
    QRect rect;

    QString getProperty(const QString& name) const;
    void addProperty(const QString& name, const QString& value);

private:
    QHash<QString, QString> properties_;
};
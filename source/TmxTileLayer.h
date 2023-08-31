#pragma once

#include <QString>
#include <QVector>

namespace Tiled
{

struct TmxTileLayer
{
    int id = 0;
    int width = 0;
    int height = 0;
    QString name;
    QVector<int> data;

    int getTileId(int x, int y);
};

}

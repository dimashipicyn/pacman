#include "TmxTileLayer.h"

#include <cassert>

namespace Tiled
{

int TmxTileLayer::getTileId(int x, int y)
{
    int index = y * width + x;
    bool isValidIndex = index >= 0 && index < data.size();
    assert(isValidIndex);
    return isValidIndex ? data[index] : -1;
}

}

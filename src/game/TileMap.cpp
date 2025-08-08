#include "TileMap.hpp"

bool TileMap::loadFromCSV(const char* /*path*/) { return true; }
bool TileMap::isSolid(int, int) const { return false; }

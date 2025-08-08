#include "TileMap.hpp"
#include <fstream>
#include <sstream>
#include <string>

bool TileMap::loadFromCSV(const char* path) {
    std::ifstream file(path);
    if (!file.is_open()) return false;
    std::string line;
    std::vector<uint8_t> tiles;
    int width = -1;
    int height = 0;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string cell;
        int w = 0;
        while (std::getline(ss, cell, ',')) {
            tiles.push_back(static_cast<uint8_t>(std::stoi(cell)));
            ++w;
        }
        if (width == -1) width = w;
        else if (w != width) return false; // inconsistent row width
        ++height;
    }
    m_width = width;
    m_height = height;
    m_tiles = std::move(tiles);
    return true;
}

bool TileMap::isSolid(int tx, int ty) const {
    if (tx < 0 || ty < 0 || tx >= m_width || ty >= m_height) return true;
    return m_tiles[ty * m_width + tx] != 0;
}

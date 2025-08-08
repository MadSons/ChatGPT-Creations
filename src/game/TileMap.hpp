#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <vector>
#include <cstdint>

// Tile-based map loaded from CSV.
class TileMap {
public:
    bool loadFromCSV(const char* path);
    bool isSolid(int tx, int ty) const;
    int width() const { return m_width; }
    int height() const { return m_height; }
private:
    int m_width{0};
    int m_height{0};
    std::vector<uint8_t> m_tiles;
};

#endif // TILEMAP_HPP

#include "tile.h"

Tile::Tile() : m_type(TileType::Floor) {}

Tile::Tile(TileType type) : m_type(type) {}

TileType Tile::type() const {
    return m_type;
}

void Tile::setType(TileType type) {
    m_type = type;
}

bool Tile::isWalkable() const {
    return m_type != TileType::Wall;
}

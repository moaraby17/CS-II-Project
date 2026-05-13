#ifndef TILE_H
#define TILE_H

// Simple list of tile types used by the dungeon grid.
enum class TileType {
    Floor,
    Wall,
    Trap,
    Goal,
    LockedDoor,
    Key
};

class Tile {
public:
    Tile();
    explicit Tile(TileType type);

    TileType type() const;
    void setType(TileType type);

    bool isWalkable() const;

private:
    TileType m_type;
};

#endif // TILE_H

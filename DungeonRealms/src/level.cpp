#include "level.h"

Level::Level() : m_width(10), m_height(10) {
    loadLevel1();
}

void Level::loadLevel1() {
    m_tiles = QVector<QVector<Tile>>(m_height, QVector<Tile>(m_width, Tile(TileType::Floor)));

    for (int x = 0; x < m_width; ++x) {
        m_tiles[0][x].setType(TileType::Wall);
        m_tiles[m_height - 1][x].setType(TileType::Wall);
    }

    for (int y = 0; y < m_height; ++y) {
        m_tiles[y][0].setType(TileType::Wall);
        m_tiles[y][m_width - 1].setType(TileType::Wall);
    }

    // Internal walls
    m_tiles[2][2].setType(TileType::Wall);
    m_tiles[2][3].setType(TileType::Wall);
    m_tiles[2][4].setType(TileType::Wall);
    m_tiles[5][5].setType(TileType::Wall);
    m_tiles[6][5].setType(TileType::Wall);
    m_tiles[7][5].setType(TileType::Wall);

    // Traps
    m_tiles[3][6].setType(TileType::Trap);
    m_tiles[4][7].setType(TileType::Trap);
    m_tiles[7][3].setType(TileType::Trap);

    // Goal tile
    m_goalPosition = QPoint(8, 8);
    m_tiles[m_goalPosition.y()][m_goalPosition.x()].setType(TileType::Goal);

    // Spawn points
    m_playerStart = QPoint(1, 1);

    m_enemies.clear();
    m_enemies.append(Enemy(6, 2));
    m_enemies.append(Enemy(7, 7));
}

int Level::width() const {
    return m_width;
}

int Level::height() const {
    return m_height;
}

bool Level::isInside(int x, int y) const {
    return x >= 0 && x < m_width && y >= 0 && y < m_height;
}

bool Level::isWalkable(int x, int y) const {
    if (!isInside(x, y)) {
        return false;
    }
    return m_tiles[y][x].isWalkable();
}

TileType Level::tileTypeAt(int x, int y) const {
    if (!isInside(x, y)) {
        return TileType::Wall;
    }
    return m_tiles[y][x].type();
}

QPoint Level::playerStart() const {
    return m_playerStart;
}

QPoint Level::goalPosition() const {
    return m_goalPosition;
}

QVector<Enemy>& Level::enemies() {
    return m_enemies;
}

const QVector<Enemy>& Level::enemies() const {
    return m_enemies;
}

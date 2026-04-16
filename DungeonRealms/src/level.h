#ifndef LEVEL_H
#define LEVEL_H

#include <QPoint>
#include <QVector>

#include "enemy.h"
#include "tile.h"

class Level {
public:
    Level();

    void loadLevel1();

    int width() const;
    int height() const;

    bool isInside(int x, int y) const;
    bool isWalkable(int x, int y) const;
    TileType tileTypeAt(int x, int y) const;

    QPoint playerStart() const;
    QPoint goalPosition() const;

    QVector<Enemy>& enemies();
    const QVector<Enemy>& enemies() const;

private:
    int m_width;
    int m_height;
    QVector<QVector<Tile>> m_tiles;
    QPoint m_playerStart;
    QPoint m_goalPosition;
    QVector<Enemy> m_enemies;
};

#endif // LEVEL_H

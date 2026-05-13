#ifndef LEVEL_H
#define LEVEL_H

#include <QPoint>
#include <QString>
#include <QVector>

#include "enemy.h"
#include "tile.h"

class Level {
public:
    Level();

    void loadLevel(int levelNumber);

    int width() const;
    int height() const;
    int number() const;
    QString name() const;

    bool isInside(int x, int y) const;
    bool isWalkable(int x, int y) const;
    TileType tileTypeAt(int x, int y) const;
    void setTileTypeAt(int x, int y, TileType type);

    QPoint playerStart() const;
    QPoint goalPosition() const;

    QVector<Enemy>& enemies();
    const QVector<Enemy>& enemies() const;

private:
    void loadFromMap(const QVector<QString>& rows, const QString& name);

    int m_width;
    int m_height;
    int m_number;
    QString m_name;
    QVector<QVector<Tile>> m_tiles;
    QPoint m_playerStart;
    QPoint m_goalPosition;
    QVector<Enemy> m_enemies;
};

#endif // LEVEL_H

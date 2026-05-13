#include "level.h"

Level::Level() : m_width(0), m_height(0), m_number(1) {
    loadLevel(1);
}

void Level::loadLevel(int levelNumber) {
    m_number = levelNumber;

    // Legend: # wall, . floor, P player, E basic enemy, A archer,
    // B brute, ^ trap, K key, D locked door, X exit/treasure.
    switch (levelNumber) {
    case 1:
        loadFromMap({
                        "##########",
                        "#P...E...#",
                        "#..##....#",
                        "#.....^..#",
                        "#........#",
                        "#....#...#",
                        "#....#...#",
                        "#..^.#.E.#",
                        "#.......X#",
                        "##########"},
                    "Training Halls");
        break;
    case 2:
        loadFromMap({
                        "##########",
                        "#P..#....#",
                        "#.#.#.E..#",
                        "#.#...#..#",
                        "#...^.#A.#",
                        "###.#....#",
                        "#...#.^..#",
                        "#.E...#..#",
                        "#.....#.X#",
                        "##########"},
                    "Guarded Corridors");
        break;
    case 3:
        loadFromMap({
                        "##########",
                        "#P..#..K.#",
                        "#.#.#.##.#",
                        "#.#...A..#",
                        "#...^....#",
                        "###D######",
                        "#....B...#",
                        "#.E..^...#",
                        "#.......X#",
                        "##########"},
                    "Locked Vault");
        break;
    case 4:
        loadFromMap({
                        "##########",
                        "#P.^..A..#",
                        "#.#.####.#",
                        "#.#....#.#",
                        "#.###^.#.#",
                        "#...E..#.#",
                        "###.##...#",
                        "#B..^..E.#",
                        "#......#X#",
                        "##########"},
                    "Trap Maze");
        break;
    default:
        loadFromMap({
                        "##########",
                        "#P..^..A.#",
                        "#.####.#.#",
                        "#....#.#.#",
                        "#.##.#...#",
                        "#K.#.###D#",
                        "#..#..B..#",
                        "#A.^.E.^.#",
                        "#.....B.X#",
                        "##########"},
                    "Dragon's Treasury");
        break;
    }
}

void Level::loadFromMap(const QVector<QString>& rows, const QString& name) {
    m_name = name;
    m_height = rows.size();
    m_width = rows.isEmpty() ? 0 : rows.first().size();
    m_tiles = QVector<QVector<Tile>>(m_height, QVector<Tile>(m_width, Tile(TileType::Floor)));
    m_enemies.clear();
    m_playerStart = QPoint(1, 1);
    m_goalPosition = QPoint(m_width - 2, m_height - 2);

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            const QChar marker = rows[y][x];
            switch (marker.toLatin1()) {
            case '#':
                m_tiles[y][x].setType(TileType::Wall);
                break;
            case '^':
                m_tiles[y][x].setType(TileType::Trap);
                break;
            case 'K':
                m_tiles[y][x].setType(TileType::Key);
                break;
            case 'D':
                m_tiles[y][x].setType(TileType::LockedDoor);
                break;
            case 'X':
                m_tiles[y][x].setType(TileType::Goal);
                m_goalPosition = QPoint(x, y);
                break;
            case 'P':
                m_playerStart = QPoint(x, y);
                break;
            case 'E':
                m_enemies.append(Enemy(x, y, EnemyType::Basic));
                break;
            case 'A':
                m_enemies.append(Enemy(x, y, EnemyType::Archer));
                break;
            case 'B':
                m_enemies.append(Enemy(x, y, EnemyType::Brute));
                break;
            default:
                break;
            }
        }
    }
}

int Level::width() const {
    return m_width;
}

int Level::height() const {
    return m_height;
}

int Level::number() const {
    return m_number;
}

QString Level::name() const {
    return m_name;
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

void Level::setTileTypeAt(int x, int y, TileType type) {
    if (isInside(x, y)) {
        m_tiles[y][x].setType(type);
    }
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

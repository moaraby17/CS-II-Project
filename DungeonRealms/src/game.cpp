#include "game.h"

#include <QtGlobal>

Game::Game(QObject* parent)
    : QObject(parent),
      m_statusText("Use arrow keys or WASD to move. Defeat enemies, then reach the treasure!"),
      m_currentLevel(1),
      m_gameWon(false) {
    startGame();
}

void Game::startGame() {
    m_gameWon = false;
    loadLevel(1);
}

void Game::loadLevel(int levelNumber) {
    m_currentLevel = qBound(1, levelNumber, TOTAL_LEVELS);
    m_level.loadLevel(m_currentLevel);
    m_player = Player();
    m_player.reset(m_level.playerStart().x(), m_level.playerStart().y());
    m_statusText = QString("Level %1: %2. Defeat all enemies, then step on the treasure.")
                       .arg(m_currentLevel)
                       .arg(m_level.name());
    emit gameStateChanged();
}

void Game::handlePlayerMove(int dx, int dy) {
    if (m_gameWon) {
        return;
    }

    playerTurn(dx, dy);
    emit gameStateChanged();
}

void Game::skipToNextLevel() {
    if (m_gameWon) {
        return;
    }

    if (m_currentLevel >= TOTAL_LEVELS) {
        m_gameWon = true;
        m_statusText = "Victory! You cleared all five dungeon levels.";
        emit gameStateChanged();
        emit gameWon();
        return;
    }

    const int nextLevel = m_currentLevel + 1;
    loadLevel(nextLevel);
    m_statusText = QString("Demo shortcut used. Jumped to Level %1.").arg(nextLevel);
    emit gameStateChanged();
}

void Game::handlePlayerAttack() {
    if (m_gameWon) {
        return;
    }

    // Attack the first adjacent enemy. This keeps combat simple for keyboard play.
    const int directions[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
    for (const auto& direction : directions) {
        int enemyIndex = -1;
        const int x = m_player.x() + direction[0];
        const int y = m_player.y() + direction[1];
        if (isEnemyAt(x, y, &enemyIndex)) {
            emit projectileFired(m_player.x(), m_player.y(), x, y, true);
            attackEnemyAt(enemyIndex);
            if (!checkAndHandleDeath()) {
                enemyTurn();
                checkAndHandleDeath();
            }
            emit gameStateChanged();
            return;
        }
    }

    m_statusText = "No adjacent enemy to attack. Move next to one or move into it.";
    emit gameStateChanged();
}

const Level& Game::level() const {
    return m_level;
}

const Player& Game::player() const {
    return m_player;
}

QString Game::statusText() const {
    return m_statusText;
}

int Game::currentLevel() const {
    return m_currentLevel;
}

int Game::totalLevels() const {
    return TOTAL_LEVELS;
}

int Game::enemiesRemaining() const {
    int count = 0;
    for (const Enemy& enemy : m_level.enemies()) {
        if (enemy.isAlive()) {
            ++count;
        }
    }
    return count;
}

bool Game::isGameWon() const {
    return m_gameWon;
}

bool Game::isEnemyAt(int x, int y, int* indexOut) const {
    const auto& enemies = m_level.enemies();
    for (int i = 0; i < enemies.size(); ++i) {
        if (enemies[i].isAlive() && enemies[i].x() == x && enemies[i].y() == y) {
            if (indexOut) {
                *indexOut = i;
            }
            return true;
        }
    }
    return false;
}

bool Game::isBlockedForEnemy(int x, int y, const Enemy* movingEnemy) const {
    if (!m_level.isWalkable(x, y) || (m_player.x() == x && m_player.y() == y)) {
        return true;
    }

    for (const Enemy& other : m_level.enemies()) {
        if (&other != movingEnemy && other.isAlive() && other.x() == x && other.y() == y) {
            return true;
        }
    }

    return false;
}

bool Game::hasLineOfSight(const Enemy& enemy) const {
    if (enemy.x() != m_player.x() && enemy.y() != m_player.y()) {
        return false;
    }

    const int stepX = (m_player.x() > enemy.x()) ? 1 : (m_player.x() < enemy.x() ? -1 : 0);
    const int stepY = (m_player.y() > enemy.y()) ? 1 : (m_player.y() < enemy.y() ? -1 : 0);
    int x = enemy.x() + stepX;
    int y = enemy.y() + stepY;

    while (x != m_player.x() || y != m_player.y()) {
        const TileType tile = m_level.tileTypeAt(x, y);
        if (tile == TileType::Wall || tile == TileType::LockedDoor || isEnemyAt(x, y)) {
            return false;
        }
        x += stepX;
        y += stepY;
    }

    return true;
}

bool Game::tryMoveEnemy(Enemy& enemy, int stepX, int stepY) {
    const int nextX = enemy.x() + stepX;
    const int nextY = enemy.y() + stepY;
    if (!isBlockedForEnemy(nextX, nextY, &enemy)) {
        enemy.setPosition(nextX, nextY);
        return true;
    }
    return false;
}

void Game::playerTurn(int dx, int dy) {
    const int targetX = m_player.x() + dx;
    const int targetY = m_player.y() + dy;
    const TileType targetTile = m_level.tileTypeAt(targetX, targetY);

    if (targetTile == TileType::LockedDoor) {
        if (m_player.hasKey()) {
            m_level.setTileTypeAt(targetX, targetY, TileType::Floor);
            m_player.useKey();
            m_statusText = "You unlocked the door with the key.";
        } else {
            m_statusText = "The door is locked. Find the key first.";
            return;
        }
    }

    if (!m_level.isWalkable(targetX, targetY)) {
        m_statusText = "Bumped into a wall.";
        return;
    }

    int enemyIndex = -1;
    if (isEnemyAt(targetX, targetY, &enemyIndex)) {
        emit projectileFired(m_player.x(), m_player.y(), targetX, targetY, true);
        attackEnemyAt(enemyIndex);
    } else {
        m_player.setPosition(targetX, targetY);
        applyTileEffectToPlayer();
    }

    if (checkAndHandleDeath()) {
        return;
    }

    if (areAllEnemiesDefeated() && isPlayerAtGoal()) {
        advanceAfterGoal();
        return;
    }

    enemyTurn();

    if (checkAndHandleDeath()) {
        return;
    }

    if (areAllEnemiesDefeated() && isPlayerAtGoal()) {
        advanceAfterGoal();
    }
}

void Game::attackEnemyAt(int enemyIndex) {
    Enemy& enemy = m_level.enemies()[enemyIndex];
    enemy.takeDamage(m_player.attackDamage());
    if (!enemy.isAlive()) {
        m_statusText = QString("Defeated a %1! %2 enemies remain.")
                           .arg(enemy.displayLetter() == "B" ? "brute" : enemy.displayLetter() == "A" ? "ranged enemy" : "goblin")
                           .arg(enemiesRemaining());
    } else {
        m_statusText = QString("You hit an enemy. It has %1 HP left.").arg(enemy.health());
    }
}

void Game::enemyTurn() {
    auto& enemies = m_level.enemies();

    for (Enemy& enemy : enemies) {
        if (!enemy.isAlive()) {
            continue;
        }

        const int dx = m_player.x() - enemy.x();
        const int dy = m_player.y() - enemy.y();
        const int manhattan = qAbs(dx) + qAbs(dy);

        if (manhattan == 1) {
            m_player.takeDamage(enemy.attackDamage());
            m_statusText = "An enemy hits you in melee!";
            continue;
        }

        if (enemy.type() == EnemyType::Archer && hasLineOfSight(enemy)) {
            emit projectileFired(enemy.x(), enemy.y(), m_player.x(), m_player.y(), false);
            m_player.takeDamage(enemy.attackDamage());
            m_statusText = "An archer fires a projectile at you!";
            continue;
        }

        const int primaryX = (dx > 0) ? 1 : (dx < 0 ? -1 : 0);
        const int primaryY = (dy > 0) ? 1 : (dy < 0 ? -1 : 0);

        // Try the direction with the bigger distance first, then try the other axis.
        if (qAbs(dx) >= qAbs(dy)) {
            if (!tryMoveEnemy(enemy, primaryX, 0)) {
                tryMoveEnemy(enemy, 0, primaryY);
            }
        } else {
            if (!tryMoveEnemy(enemy, 0, primaryY)) {
                tryMoveEnemy(enemy, primaryX, 0);
            }
        }
    }
}

void Game::applyTileEffectToPlayer() {
    const TileType tile = m_level.tileTypeAt(m_player.x(), m_player.y());

    if (tile == TileType::Trap) {
        m_player.takeDamage(2);
        m_statusText = "You stepped on a trap! (-2 HP)";
    } else if (tile == TileType::Key) {
        m_player.pickUpKey();
        m_level.setTileTypeAt(m_player.x(), m_player.y(), TileType::Floor);
        m_statusText = "You picked up a key. It can open one locked door.";
    } else if (tile == TileType::Goal) {
        if (areAllEnemiesDefeated()) {
            m_statusText = "The treasure glows. Level complete!";
        } else {
            m_statusText = "Defeat all enemies before using the treasure exit.";
        }
    } else {
        m_statusText = "Player moved. Enemies take their turn.";
    }
}

bool Game::checkAndHandleDeath() {
    if (!m_player.isAlive()) {
        const int restartLevel = m_currentLevel;
        emit playerDied(restartLevel);
        m_level.loadLevel(restartLevel);
        m_player = Player();
        m_player.reset(m_level.playerStart().x(), m_level.playerStart().y());
        m_statusText = QString("You died! Level %1 restarted from the beginning.").arg(restartLevel);
        emit gameStateChanged();
        return true;
    }
    return false;
}

bool Game::areAllEnemiesDefeated() const {
    return enemiesRemaining() == 0;
}

bool Game::isPlayerAtGoal() const {
    return m_player.x() == m_level.goalPosition().x() && m_player.y() == m_level.goalPosition().y();
}

void Game::advanceAfterGoal() {
    const int completedLevel = m_currentLevel;
    emit levelCompleted(completedLevel);

    if (m_currentLevel >= TOTAL_LEVELS) {
        m_gameWon = true;
        m_statusText = "Victory! You cleared all five dungeon levels.";
        emit gameStateChanged();
        emit gameWon();
        return;
    }

    loadLevel(m_currentLevel + 1);
}

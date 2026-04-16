#include "game.h"

#include <QtGlobal>

Game::Game(QObject* parent)
    : QObject(parent),
      m_statusText("Use arrow keys to move. Defeat enemies and reach the goal!"),
      m_levelComplete(false) {
    startLevel1();
}

void Game::startLevel1() {
    m_level.loadLevel1();
    m_player = Player();
    m_player.setPosition(m_level.playerStart().x(), m_level.playerStart().y());
    m_statusText = "Level 1 started. Arrow keys: move/attack.";
    m_levelComplete = false;
    emit gameStateChanged();
}

void Game::handlePlayerMove(int dx, int dy) {
    if (m_levelComplete) {
        return;
    }

    playerTurn(dx, dy);
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

bool Game::isLevelComplete() const {
    return m_levelComplete;
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

void Game::playerTurn(int dx, int dy) {
    const int targetX = m_player.x() + dx;
    const int targetY = m_player.y() + dy;

    if (!m_level.isWalkable(targetX, targetY)) {
        m_statusText = "Bumped into a wall.";
        return;
    }

    int enemyIndex = -1;
    if (isEnemyAt(targetX, targetY, &enemyIndex)) {
        auto& enemy = m_level.enemies()[enemyIndex];
        enemy.takeDamage(m_player.attackDamage());
        if (!enemy.isAlive()) {
            m_statusText = "Enemy defeated!";
        } else {
            m_statusText = "You hit an enemy.";
        }
    } else {
        m_player.setPosition(targetX, targetY);
        applyTrapDamageToPlayer();
    }

    if (checkAndHandleDeath()) {
        return;
    }

    enemyTurn();

    if (checkAndHandleDeath()) {
        return;
    }

    if (areAllEnemiesDefeated() && isPlayerAtGoal()) {
        m_levelComplete = true;
        m_statusText = "Level 1 Complete! You cleared the dungeon.";
        emit levelCompleted();
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
            m_statusText = "An enemy hits you!";
            continue;
        }

        int stepX = 0;
        int stepY = 0;

        if (qAbs(dx) >= qAbs(dy)) {
            stepX = (dx > 0) ? 1 : (dx < 0 ? -1 : 0);
        } else {
            stepY = (dy > 0) ? 1 : (dy < 0 ? -1 : 0);
        }

        int nextX = enemy.x() + stepX;
        int nextY = enemy.y() + stepY;

        bool occupiedByEnemy = false;
        for (const Enemy& other : enemies) {
            if (&other != &enemy && other.isAlive() && other.x() == nextX && other.y() == nextY) {
                occupiedByEnemy = true;
                break;
            }
        }

        if (m_level.isWalkable(nextX, nextY)
            && !(m_player.x() == nextX && m_player.y() == nextY)
            && !occupiedByEnemy) {
            enemy.setPosition(nextX, nextY);
        }
    }
}

void Game::applyTrapDamageToPlayer() {
    if (m_level.tileTypeAt(m_player.x(), m_player.y()) == TileType::Trap) {
        m_player.takeDamage(2);
        m_statusText = "You stepped on a trap! (-2 HP)";
    }
}

bool Game::checkAndHandleDeath() {
    if (!m_player.isAlive()) {
        m_statusText = "You died! Restarting Level 1...";
        emit gameStateChanged();
        startLevel1();
        return true;
    }
    return false;
}

bool Game::areAllEnemiesDefeated() const {
    for (const Enemy& enemy : m_level.enemies()) {
        if (enemy.isAlive()) {
            return false;
        }
    }
    return true;
}

bool Game::isPlayerAtGoal() const {
    return m_player.x() == m_level.goalPosition().x() && m_player.y() == m_level.goalPosition().y();
}

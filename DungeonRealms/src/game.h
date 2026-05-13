#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QString>

#include "level.h"
#include "player.h"

class Game : public QObject {
    Q_OBJECT
public:
    explicit Game(QObject* parent = nullptr);

    void startGame();
    void loadLevel(int levelNumber);
    void handlePlayerMove(int dx, int dy);
    void handlePlayerAttack();
    void skipToNextLevel();

    const Level& level() const;
    const Player& player() const;

    QString statusText() const;
    int currentLevel() const;
    int totalLevels() const;
    int enemiesRemaining() const;
    bool isGameWon() const;

signals:
    void gameStateChanged();
    void levelCompleted(int levelNumber);
    void gameWon();
    void playerDied(int levelNumber);
    void projectileFired(int fromX, int fromY, int toX, int toY, bool fromPlayer);

private:
    bool isEnemyAt(int x, int y, int* indexOut = nullptr) const;
    bool isBlockedForEnemy(int x, int y, const Enemy* movingEnemy) const;
    bool hasLineOfSight(const Enemy& enemy) const;
    bool tryMoveEnemy(Enemy& enemy, int stepX, int stepY);
    void playerTurn(int dx, int dy);
    void attackEnemyAt(int enemyIndex);
    void enemyTurn();
    void applyTileEffectToPlayer();
    bool checkAndHandleDeath();
    bool areAllEnemiesDefeated() const;
    bool isPlayerAtGoal() const;
    void advanceAfterGoal();

    static constexpr int TOTAL_LEVELS = 5;

    Level m_level;
    Player m_player;
    QString m_statusText;
    int m_currentLevel;
    bool m_gameWon;
};

#endif // GAME_H

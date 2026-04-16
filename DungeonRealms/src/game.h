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

    void startLevel1();
    void handlePlayerMove(int dx, int dy);

    const Level& level() const;
    const Player& player() const;

    QString statusText() const;
    bool isLevelComplete() const;

signals:
    void gameStateChanged();
    void levelCompleted();

private:
    bool isEnemyAt(int x, int y, int* indexOut = nullptr) const;
    void playerTurn(int dx, int dy);
    void enemyTurn();
    void applyTrapDamageToPlayer();
    bool checkAndHandleDeath();
    bool areAllEnemiesDefeated() const;
    bool isPlayerAtGoal() const;

    Level m_level;
    Player m_player;
    QString m_statusText;
    bool m_levelComplete;
};

#endif // GAME_H

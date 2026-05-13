#ifndef ENEMY_H
#define ENEMY_H

#include <QString>

#include "entity.h"

enum class EnemyType {
    Basic,
    Archer,
    Brute
};

class Enemy : public Entity {
public:
    Enemy();
    Enemy(int x, int y, EnemyType type = EnemyType::Basic);

    int attackDamage() const;
    EnemyType type() const;
    QString displayLetter() const;

private:
    int m_attackDamage;
    EnemyType m_type;
};

#endif // ENEMY_H

#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"

class Enemy : public Entity {
public:
    Enemy();
    Enemy(int x, int y);

    int attackDamage() const;

private:
    int m_attackDamage;
};

#endif // ENEMY_H

#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

class Player : public Entity {
public:
    Player();

    int attackDamage() const;

private:
    int m_attackDamage;
};

#endif // PLAYER_H

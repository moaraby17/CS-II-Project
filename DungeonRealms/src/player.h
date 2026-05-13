#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

class Player : public Entity {
public:
    Player();

    int attackDamage() const;
    bool hasKey() const;
    void pickUpKey();
    void useKey();
    void reset(int x, int y);

private:
    int m_attackDamage;
    bool m_hasKey;
};

#endif // PLAYER_H

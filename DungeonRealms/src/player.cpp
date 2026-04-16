#include "player.h"

Player::Player() : Entity(1, 1, 10), m_attackDamage(2) {}

int Player::attackDamage() const {
    return m_attackDamage;
}

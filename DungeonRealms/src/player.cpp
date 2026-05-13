#include "player.h"

Player::Player() : Entity(1, 1, 14), m_attackDamage(3), m_hasKey(false) {}

int Player::attackDamage() const {
    return m_attackDamage;
}

bool Player::hasKey() const {
    return m_hasKey;
}

void Player::pickUpKey() {
    m_hasKey = true;
}

void Player::useKey() {
    m_hasKey = false;
}

void Player::reset(int x, int y) {
    m_x = x;
    m_y = y;
    m_health = m_maxHealth;
    m_hasKey = false;
}

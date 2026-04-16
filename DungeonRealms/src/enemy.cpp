#include "enemy.h"

Enemy::Enemy() : Entity(0, 0, 4), m_attackDamage(2) {}

Enemy::Enemy(int x, int y) : Entity(x, y, 4), m_attackDamage(2) {}

int Enemy::attackDamage() const {
    return m_attackDamage;
}

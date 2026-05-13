#include "enemy.h"

namespace {
int startingHealthFor(EnemyType type) {
    switch (type) {
    case EnemyType::Basic:
        return 4;
    case EnemyType::Archer:
        return 3;
    case EnemyType::Brute:
        return 7;
    }
    return 4;
}

int damageFor(EnemyType type) {
    switch (type) {
    case EnemyType::Basic:
        return 2;
    case EnemyType::Archer:
        return 2;
    case EnemyType::Brute:
        return 3;
    }
    return 2;
}
}

Enemy::Enemy() : Enemy(0, 0, EnemyType::Basic) {}

Enemy::Enemy(int x, int y, EnemyType type)
    : Entity(x, y, startingHealthFor(type)),
      m_attackDamage(damageFor(type)),
      m_type(type) {}

int Enemy::attackDamage() const {
    return m_attackDamage;
}

EnemyType Enemy::type() const {
    return m_type;
}

QString Enemy::displayLetter() const {
    switch (m_type) {
    case EnemyType::Basic:
        return "E";
    case EnemyType::Archer:
        return "A";
    case EnemyType::Brute:
        return "B";
    }
    return "E";
}

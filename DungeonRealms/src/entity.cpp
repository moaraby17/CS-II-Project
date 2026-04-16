#include "entity.h"

Entity::Entity(int x, int y, int health)
    : m_x(x), m_y(y), m_health(health), m_maxHealth(health) {}

int Entity::x() const {
    return m_x;
}

int Entity::y() const {
    return m_y;
}

void Entity::setPosition(int x, int y) {
    m_x = x;
    m_y = y;
}

int Entity::health() const {
    return m_health;
}

int Entity::maxHealth() const {
    return m_maxHealth;
}

void Entity::setHealth(int health) {
    m_health = health;
    if (m_health > m_maxHealth) {
        m_health = m_maxHealth;
    }
}

void Entity::takeDamage(int amount) {
    m_health -= amount;
}

bool Entity::isAlive() const {
    return m_health > 0;
}

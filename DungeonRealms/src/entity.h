#ifndef ENTITY_H
#define ENTITY_H

class Entity {
public:
    Entity(int x = 0, int y = 0, int health = 1);
    virtual ~Entity() = default;

    int x() const;
    int y() const;
    void setPosition(int x, int y);

    int health() const;
    int maxHealth() const;
    void setHealth(int health);
    void takeDamage(int amount);
    bool isAlive() const;

protected:
    int m_x;
    int m_y;
    int m_health;
    int m_maxHealth;
};

#endif // ENTITY_H
